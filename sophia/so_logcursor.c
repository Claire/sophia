
/*
 * sophia database
 * sphia.org
 *
 * Copyright (c) Dmitry Simonenko
 * BSD License
*/

#include <libsr.h>
#include <libsv.h>
#include <libsx.h>
#include <libsl.h>
#include <libsd.h>
#include <libsi.h>
#include <libse.h>
#include <libso.h>

static int
so_logcursor_destroy(soobj *o)
{
	sologcursor *c = (sologcursor*)o;
	so *e = c->t->e;
	so_objindex_unregister(&c->t->logcursor, &c->o);
	sr_free(&e->a_logcursor, c);
	return 0;
}

static inline int
so_logcursor_next(sologcursor *c)
{
	if (srunlikely(c->pos == NULL))
		return 0;
	c->pos++;
	if (srunlikely(c->pos >= (svlogv*)c->t->t.log.buf.p)) {
		c->pos = NULL;
		return 0;
	}
	c->v.v = c->pos->v;
	return 1;
}

static void*
so_logcursor_get(soobj *o, va_list args srunused)
{
	sologcursor *c = (sologcursor*)o;
	if (c->ready) {
		c->ready = 0;
		return &c->v;
	}
	if (so_logcursor_next(c) == 0)
		return NULL;
	return &c->v;
}

static void*
so_logcursor_obj(soobj *obj, va_list args srunused)
{
	sologcursor *c = (sologcursor*)obj;
	if (srunlikely(c->pos == NULL))
		return NULL;
	return &c->v;
}

static void*
so_logcursor_type(soobj *o srunused, va_list args srunused) {
	return "log_cursor";
}

static soobjif sologcursorif =
{
	.ctl      = NULL,
	.open     = NULL,
	.destroy  = so_logcursor_destroy,
	.error    = NULL,
	.set      = NULL,
	.get      = so_logcursor_get,
	.del      = NULL,
	.begin    = NULL,
	.prepare  = NULL,
	.commit   = NULL,
	.rollback = NULL,
	.cursor   = NULL,
	.object   = so_logcursor_obj,
	.type     = so_logcursor_type
};

static inline void
so_logcursor_open(sologcursor *c)
{
	so_vinit(&c->v, c->t->e, NULL);
	c->v.flags = SO_VIMMUTABLE;
	c->pos = (svlogv*)c->t->t.log.buf.s;
	if (c->pos >= (svlogv*)c->t->t.log.buf.p)
		c->pos = NULL;
	if (c->pos) {
		c->v.v = c->pos->v;
		c->ready = 1;
	}
}

soobj *so_logcursor_new(sotx *t)
{
	so *e = t->e;
	sologcursor *c = sr_malloc(&e->a_logcursor, sizeof(sologcursor));
	if (srunlikely(c == NULL)) {
		sr_error(&e->error, "%s", "memory allocation failed");
		return NULL;
	}
	so_objinit(&c->o, SOLOGCURSOR, &sologcursorif, &e->o);
	c->t     = t;
	c->pos   = NULL;
	c->ready = 0;
	so_logcursor_open(c);
	so_objindex_unregister(&e->ctlcursor, &c->o);
	return &c->o;
}
