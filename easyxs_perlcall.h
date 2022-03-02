#ifndef EASYXS_PERLCALL_H
#define EASYXS_PERLCALL_H 1

#include "init.h"

static inline void _EASYXS_SET_ARGS (pTHX_ SV* object, SV** args) {
    unsigned argscount = 0;

    if (args) {
        while (args[argscount] != NULL) argscount++;
    }

    ENTER;
    SAVETMPS;

    PUSHMARK(SP);

    if (object || argscount) {
        EXTEND(SP, (object ? 1 : 0) + argscount);

        if (object) PUSHs( sv_mortalcopy(object) );

        unsigned a=0;
        while (a < argscount) mPUSHs( args[a++] );

        PUTBACK;
    }
}

#define exs_call_method_void(object, methname, args) STMT_START { \
    dSP;                                            \
                                                    \
    _EASYXS_SET_ARGS(object, args);                 \
                                                    \
    call_method( methname, G_DISCARD | G_VOID );    \
                                                    \
    FREETMPS;                                       \
    LEAVE;                                          \
} STMT_END

static inline SV* _easyxs_call_method_scalar (pTHX_ SV* object, SV** args) {
    dSP;

    _EASYXS_SET_ARGS(object, args);

    int count = call_method( methname, G_SCALAR );

    SV* ret;

    if (count == 0) {
        ret = &PL_sv_undef;
    }
    else {
        ret = SvREFCNT_inc(POPs);
    }

    FREETMPS;
    LEAVE;

    return ret;
}

#define exs_call_method_scalar(object, methname, args) \
    _easyxs_call_method_scalar(aTHX_ object, methname, args)

#endif
