#pragma once 

#include <math.h>
#include <cmath>

#undef CLASS_NAME
#define CLASS_NAME MATHLib
#undef MODULE_NAME
#define MODULE_NAME "math"
#undef MESSAGE_BODY
#define MESSAGE_BODY                                                           \
    FIELD_METHOD(sinNum, Ref<Variant>, Ref<Variant>, Ref<Variant>);                               \
    FIELD_METHOD(cosNum, Ref<Variant>, Ref<Variant>, Ref<Variant>);                                 \
    FIELD_METHOD(tanNum, Ref<Variant>, Ref<Variant>, Ref<Variant>);                                 \
    FIELD_METHOD(asinNum, Ref<Variant>, Ref<Variant>, Ref<Variant>);                \
    FIELD_METHOD(acosNum, Ref<Variant>, Ref<Variant>, Ref<Variant>);              \
    FIELD_METHOD(atanNum, Ref<Variant>, Ref<Variant>, Ref<Variant>);              \
    FIELD_METHOD(roundNum, Ref<Variant>, Ref<Variant>, integer);              



#include "reflect_header"