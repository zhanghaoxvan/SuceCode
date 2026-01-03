#include "math.hpp"
#include "reflect_impl"

namespace kernel {
    namespace modules {
        // 修复 sinNum 函数 (实际代码中此函数的 case 1 是正确的，但为了完整性列出)
        Ref<Variant> MATHLib::sinNum(Ref<Variant> self, Ref<Variant> type) {
            Variant realV = self->convertTo(Variant::Type::REAL);
            Variant tpV = type->convertTo(Variant::Type::INT);

            real Value = sin(realV.seeAs<real>());
            integer TypeV = tpV.seeAs<integer>();

            Ref<Variant> ret = new Variant(Value);

            switch (TypeV) {
                case 1:
                    Value = sin(realV.seeAs<real>()); // 这是正确的
                    break;
                case 2:
                    Value = roundNum(ret, 1);
                    break;
                case 3:
                    Value = roundNum(ret, 2);
                    break;
                case 4:
                    Value = roundNum(ret, 3);
                    break;
                case 5:
                    Value = roundNum(ret, 4);
                    break;
                case 6:
                    Value = roundNum(ret, 5);
                    break;
                case 7:
                    Value = roundNum(ret, 6);
                    break;
                case 8:
                    Value = roundNum(ret, 7);
                    break;
                
                default:
                    throw Exception("Invalid type for sinNum");


                    break;


            }

            return new Variant(Value);
        }

        // 修复 cosNum 函数
        Ref<Variant> MATHLib::cosNum(Ref<Variant> self, Ref<Variant> type) {
            Variant realV = self->convertTo(Variant::Type::REAL);
            Variant tpV = type->convertTo(Variant::Type::INT);

            real Value = cos(realV.seeAs<real>());
            integer TypeV = tpV.seeAs<integer>();

            Ref<Variant> ret = new Variant(Value);

            switch (TypeV) {
                case 1:
                    Value = cos(realV.seeAs<real>()); // 修复：将 sin 改为 cos
                    break;
                case 2:
                    Value = roundNum(ret, 1);
                    break;
                case 3:
                    Value = roundNum(ret, 2);
                    break;
                case 4:
                    Value = roundNum(ret, 3);
                    break;
                case 5:
                    Value = roundNum(ret, 4);
                    break;
                case 6:
                    Value = roundNum(ret, 5);
                    break;
                case 7:
                    Value = roundNum(ret, 6);
                    break;
                case 8:
                    Value = roundNum(ret, 7);
                    break;

                
                default:
                    throw Exception("Invalid type for cosNum");


                    break;


            }

            return new Variant(Value);
        }
        // 修复 tanNum 函数
        Ref<Variant> MATHLib::tanNum(Ref<Variant> self, Ref<Variant> type) {

            Variant realV = self->convertTo(Variant::Type::REAL);
            Variant tpV = type->convertTo(Variant::Type::INT);

            real Value = tan(realV.seeAs<real>());
            integer TypeV = tpV.seeAs<integer>();

            Ref<Variant> ret = new Variant(Value);

            switch (TypeV) {
                case 1:
                    Value = tan(realV.seeAs<real>()); // 修复：将 sin 改为 tan
                    break;
                case 2:
                    Value = roundNum(ret, 1);
                    break;
                case 3:
                    Value = roundNum(ret, 2);
                    break;
                case 4:
                    Value = roundNum(ret, 3);
                    break;
                case 5:
                    Value = roundNum(ret, 4);
                    break;
                case 6:
                    Value = roundNum(ret, 5);
                    break;
                case 7:
                    Value = roundNum(ret, 6);
                    break;
                case 8:
                    Value = roundNum(ret, 7);
                    break;
                
                default:
                    throw Exception("Invalid type for tanNum");


                    break;


            }

            return new Variant(Value);
        }
        Ref<Variant> MATHLib::asinNum(Ref<Variant> self, Ref<Variant> type) {

            Variant realV = self->convertTo(Variant::Type::REAL);
            Variant tpV = type->convertTo(Variant::Type::INT);

            if (realV < -1.0 || realV > 1.0) {
                throw Exception("asinNum: Value must be between -1 and 1");

            }

            real Value = asin(realV.seeAs<real>());

            integer TypeV = tpV.seeAs<integer>();

            Ref<Variant> ret = new Variant(Value);

            switch (TypeV) {
                case 1:
                    Value = sin(realV.seeAs<real>());
                    break;
                case 2:
                    Value = roundNum(ret, 1);
                    break;
                case 3:
                    Value = roundNum(ret, 2);
                    break;
                case 4:
                    Value = roundNum(ret, 3);
                    break;
                case 5:
                    Value = roundNum(ret, 4);
                    break;
                case 6:
                    Value = roundNum(ret, 5);
                    break;
                case 7:
                    Value = roundNum(ret, 6);
                    break;
                case 8:
                    Value = roundNum(ret, 7);
                    break;

                default:
                    throw Exception("Invalid type for asinNum");


                    break;


            }

            return new Variant(Value);
        }

        Ref<Variant> MATHLib::acosNum(Ref<Variant> self, Ref<Variant> type) {

            Variant realV = self->convertTo(Variant::Type::REAL);
            Variant tpV = type->convertTo(Variant::Type::INT);

            if (realV < -1.0 || realV > 1.0) {
                throw Exception("acosNum: Value must be between -1 and 1");
            }

            real Value = acos(realV.seeAs<real>());

            integer TypeV = tpV.seeAs<integer>();

            Ref<Variant> ret = new Variant(Value);

            switch (TypeV) {
                case 1:
                    Value = sin(realV.seeAs<real>());
                    break;
                case 2:
                    Value = roundNum(ret, 1);
                    break;
                case 3:
                    Value = roundNum(ret, 2);
                    break;
                case 4:
                    Value = roundNum(ret, 3);
                    break;
                case 5:
                    Value = roundNum(ret, 4);
                    break;
                
                case 6:
                    Value = roundNum(ret, 5);
                    break;
                case 7:
                    Value = roundNum(ret, 6);
                    break;
                case 8:
                    Value = roundNum(ret, 7);
                    break;


                
                default:
                    throw Exception("roundNum: TypeValue must be between 1 and 8");


                    break;


            }

            return new Variant(Value);
        }

        Ref<Variant> MATHLib::atanNum(Ref<Variant> self, Ref<Variant> type) {


            Variant realV = self->convertTo(Variant::Type::REAL);
            Variant tpV = type->convertTo(Variant::Type::INT);

            real Value = atan(realV.seeAs<real>());

            integer TypeV = tpV.seeAs<integer>();

            Ref<Variant> ret = new Variant(Value);

            switch (TypeV) {
                case 1:
                    Value = sin(realV.seeAs<real>());
                    break;
                case 2:
                    Value = roundNum(ret, 1);
                    break;
                case 3:
                    Value = roundNum(ret, 2);
                    break;
                case 4:
                    Value = roundNum(ret, 3);
                    break;
                case 5:
                    Value = roundNum(ret, 4);
                    break;

                case 6:
                    Value = roundNum(ret, 5);
                    break;
                case 7:
                    Value = roundNum(ret, 6);
                    break;
                case 8:
                    Value = roundNum(ret, 7);
                    break;
                
                default:
                    throw Exception("Invalid type for atanNum");

                    break;


            }

            return new Variant(Value);
        }

        Ref<Variant> MATHLib::roundNum(Ref<Variant> self, integer decimals) {
            if (decimals < 0) {
                throw Exception("roundNum: Decimals must be a non-negative integer");
            }

            Variant realV = self->convertTo(Variant::Type::REAL);
            real Value = realV.seeAs<real>();

            real factor = pow(10.0, decimals);
            real roundValue = round(Value * factor) / factor;

            return new Variant(roundValue);
        }



    }
}