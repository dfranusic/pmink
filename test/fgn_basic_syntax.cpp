#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <pmink_utils.h>
#include <fgn.h>
#include <fstream>
#include <lua_if.h>

// fgn basic matching syntax test suite v1
BOOST_AUTO_TEST_SUITE(fgn_basic_syntax_v1)

// OTT_NUMBER 
BOOST_AUTO_TEST_CASE(fgn_bs_v1_int){
    fgn::FilterManager fm;
    fgn::rule_param_t rdata;
    fgn::rule_param_t rdata_exp_res;
    // expected result
    rdata_exp_res.set_cstr( 1, "100");
    rdata_exp_res.set_int ( 1, fgn::OTT_NUMBER, 0, 1);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_cstr( 1, "100",           0, 4);

    // default operator
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = 100, OPERATOR = default (==)");
    int res = fm.prepare_match_line(&rdata, "100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // default operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]100");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = [6]100, OPERATOR = default (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);


    //  == operator
    rdata_exp_res.set_cstr( 1, "==100");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = ==100, OPERATOR = EQ (==)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "==100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    //  == operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]==100");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = [6]==100, OPERATOR = EQ (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]==100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator
    rdata_exp_res.set_cstr( 1, "!=100");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = !=100, OPERATOR = NE (!=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "!=100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]!=100");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = [6]!=100, OPERATOR = NE (!=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]!=100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator
    rdata_exp_res.set_cstr( 1, ">100");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = >100, OPERATOR = GT (>)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]>100");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = [6]>100, OPERATOR = GT (>), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >= operator
    rdata_exp_res.set_cstr( 1, ">=100");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = >=100, OPERATOR = GTE (>=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">=100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >= operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]>=100");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = [6]>=100, OPERATOR = GTE (>=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>=100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator
    rdata_exp_res.set_cstr( 1, "<100");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = <100, OPERATOR = LT (<)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]<100");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = [6]<100, OPERATOR = LT (<), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator
    rdata_exp_res.set_cstr( 1, "<=100");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = <=100, OPERATOR = LTE (<=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<=100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]<=100");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing OTT_NUMBER OperandType: VALUE = [6]<=100, OPERATOR = LTE (<=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<=100", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);
}

// OTT_STRING and OTT_LUA
BOOST_AUTO_TEST_CASE(fgn_bs_v1_str_lua){
    fgn::FilterManager fm;
    fgn::rule_param_t rdata;
    fgn::rule_param_t rdata_exp_res;
    // expected result
    rdata_exp_res.set_cstr( 1, "test string");
    rdata_exp_res.set_int ( 1, fgn::OTT_STRING, 0, 1);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_cstr( 1, "test string",   0, 4);

    // default operator
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = test string, OPERATOR = default (==)");
    int res = fm.prepare_match_line(&rdata, "test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // default operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = [6]test string, OPERATOR = default (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator
    rdata_exp_res.set_cstr( 1, "==test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = ==test string, OPERATOR = EQ (==)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "==test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]==test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = [6]==test string, OPERATOR = EQ (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]==test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator
    rdata_exp_res.set_cstr( 1, "!=test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = !=test string, OPERATOR = NE (!=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "!=test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]!=test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = [6]!=test string, OPERATOR = NE (!=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]!=test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator
    rdata_exp_res.set_cstr( 1, ">test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = >test string, OPERATOR = GT (>)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]>test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = [6]>test string, OPERATOR = GT (>), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >= operator
    rdata_exp_res.set_cstr( 1, ">=test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = >=test string, OPERATOR = GTE (>=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">=test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

     // >= operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]>=test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = [6]>=test string, OPERATOR = GTE (>=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>=test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator
    rdata_exp_res.set_cstr( 1, "<test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = <test string, OPERATOR = LT (<)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]<test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = [6]<test string, OPERATOR = LT (<), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator
    rdata_exp_res.set_cstr( 1, "<=test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = <=test string, OPERATOR = LTE (<=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<=test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]<=test string");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_STRING OperandType: VALUE = [6]<=test string, OPERATOR = LTE (<=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<=test string", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // lua inline
    rdata_exp_res.clear_params();
    rdata_exp_res.set_cstr( 1, "``return true");
    rdata_exp_res.set_int ( 1, fgn::OTT_LUA,    0, 1);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LUA (inline) OperandType: VALUE = ``return true");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "``return true", 1);
    BOOST_CHECK(res == 0);
    BOOST_CHECK(rdata.get_param(1, 0, 1) != NULL);
    BOOST_CHECK((int)*rdata.get_param(1, 0, 1) == fgn::OTT_LUA);
    // copy pointers
    rdata_exp_res.set(rdata, pmink_utils::ParamTuple<>(1, 0, 2), pmink_utils::ParamTuple<>(1, 0, 2));
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);
    // check lua script data
    std::string lua_gen_str(PMINK_LUA_MODULE);
    lua_gen_str.append("return true");
    std::string* str_p = (std::string*)(void*)*rdata.get_param(1, 0, 2);
    BOOST_CHECK(lua_gen_str == *str_p);

    // lua external
    std::ofstream ofs("/tmp/pmink_test.lua", std::ofstream::out);
    ofs << "return true\n";
    ofs.close();
    rdata_exp_res.clear_params();
    rdata_exp_res.set_cstr( 1, "``/tmp/pmink_test.lua");
    rdata_exp_res.set_int ( 1, fgn::OTT_LUA,    0, 1);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LUA (external) OperandType: VALUE = ``/tmp/pmink_test.lua");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "``/tmp/pmink_test.lua", 1);
    BOOST_CHECK(res == 0);
    BOOST_CHECK(rdata.get_param(1, 0, 1) != NULL);
    BOOST_CHECK((int)*rdata.get_param(1, 0, 1) == fgn::OTT_LUA);
    // copy pointers
    rdata_exp_res.set(rdata, pmink_utils::ParamTuple<>(1, 0, 2), pmink_utils::ParamTuple<>(1, 0, 2));
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);
    // check lua script data
    str_p = (std::string*)(void*)*rdata.get_param(1, 0, 2);
    BOOST_CHECK("return true\n" == *str_p);
    // cleanup
    std::remove("/tmp/pmink_test.lua");
}

// OTT_REGEX
BOOST_AUTO_TEST_CASE(fgn_bs_v1_rgx){
    fgn::FilterManager fm;
    fgn::rule_param_t rdata;
    fgn::rule_param_t rdata_exp_res;
    // expected result
    rdata_exp_res.set_cstr( 1, ":...abc efg*");
    rdata_exp_res.set_int ( 1, fgn::OTT_REGEX,  0, 1);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_cstr( 1, "...abc efg*",   0, 3);

    // without LCI
    BOOST_TEST_MESSAGE("Testing fgn::OTT_REGEX OperandType: VALUE = :...abc efg*");
    int res = fm.prepare_match_line(&rdata, ":...abc efg*", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // with LCI
    rdata_exp_res.set_cstr( 1, "[6]:...abc efg*");
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_REGEX OperandType: VALUE = [6]:...abc efg*, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]:...abc efg*", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);
}

// OTT_VARIABLE
BOOST_AUTO_TEST_CASE(fgn_bs_v1_var){
    fgn::FilterManager fm;
    fgn::rule_param_t rdata;
    fgn::rule_param_t rdata_exp_res;
    // expected result
    rdata_exp_res.set_cstr( 1, "@test_var");
    rdata_exp_res.set_int ( 1, fgn::OTT_VARIABLE,   0, 1);
    rdata_exp_res.set_int ( 1, 0,                   0, 2);
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,         0, 3);
    rdata_exp_res.set_int ( 1, 0,                   0, 4);
    rdata_exp_res.set_cstr( 1, "test_var",          0, 5);

    // default operator
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = @test_var, OPERATOR = default (==)");
    int res = fm.prepare_match_line(&rdata, "@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // default operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]@test_var, OPERATOR = default (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // default operator with RCI
    rdata_exp_res.set_cstr( 1, "@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = @[7]test_var, OPERATOR = default (==), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // default operator with RCI and LCI
    rdata_exp_res.set_cstr( 1, "[6]@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]@[7]test_var, OPERATOR = default (==), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator
    rdata_exp_res.set_cstr( 1, "==@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = ==@test_var, OPERATOR = EQ (==)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "==@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]==@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]==@test_var, OPERATOR = EQ (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]==@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator with RCI
    rdata_exp_res.set_cstr( 1, "==@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = ==@[7]test_var, OPERATOR = EQ (==), RCI = 7");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "==@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator with RCI and LCI
    rdata_exp_res.set_cstr( 1, "[6]==@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]==@[7]test_var, OPERATOR = EQ (==), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]==@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator
    rdata_exp_res.set_cstr( 1, "!=@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = !=@test_var, OPERATOR = NE (!=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "!=@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]!=@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]!=@test_var, OPERATOR = NE (!=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]!=@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator with RCI
    rdata_exp_res.set_cstr( 1, "!=@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = !=@[7]test_var, OPERATOR = NE (!=), RCI = 7");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "!=@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator with RCI and LCI
    rdata_exp_res.set_cstr( 1, "[6]!=@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]!=@[7]test_var, OPERATOR = NE (!=), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]!=@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator
    rdata_exp_res.set_cstr( 1, ">@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = >@test_var, OPERATOR = GT (>)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]>@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]>@test_var, OPERATOR = GT (>), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator with RCI
    rdata_exp_res.set_cstr( 1, ">@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = >@[7]test_var, OPERATOR = GT (>), RCI = 7");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // > operator with RCI and LCI
    rdata_exp_res.set_cstr( 1, "[6]>@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]>@[7]test_var, OPERATOR = GT (>), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >= operator
    rdata_exp_res.set_cstr( 1, ">=@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = >=@test_var, OPERATOR = GTE (>=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">=@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >= operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]>=@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]>=@test_var, OPERATOR = GTE (>=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>=@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >= operator with RCI
    rdata_exp_res.set_cstr( 1, ">=@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = >=@[7]test_var, OPERATOR = GTE (>=), RCI = 7");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">=@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >= operator with RCI and LCI
    rdata_exp_res.set_cstr( 1, "[6]>=@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_GTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]>=@[7]test_var, OPERATOR = GTE (>=), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>=@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator
    rdata_exp_res.set_cstr( 1, "<@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = <@test_var, OPERATOR = LT (<)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]<@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]<@test_var, OPERATOR = LT (<), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator with RCI
    rdata_exp_res.set_cstr( 1, "<@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = <@[7]test_var, OPERATOR = LT (<), RCI = 7");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // < operator with RCI and LCI
    rdata_exp_res.set_cstr( 1, "[6]<@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LT,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]<@[7]test_var, OPERATOR = LT (<), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator
    rdata_exp_res.set_cstr( 1, "<=@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = <=@test_var, OPERATOR = LTE (<=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<=@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]<=@test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 0,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]<=@test_var, OPERATOR = LTE (<=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<=@test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator with RCI
    rdata_exp_res.set_cstr( 1, "<=@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = <=@[7]test_var, OPERATOR = LTE (<=), RCI = 7");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<=@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // <= operator with RCI and LCI
    rdata_exp_res.set_cstr( 1, "[6]<=@[7]test_var");
    rdata_exp_res.set_int ( 1, fgn::ROT_LTE,    0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    rdata_exp_res.set_int ( 1, 7,               0, 4);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_VARIABLE OperandType: VALUE = [6]<=@[7]test_var, OPERATOR = LTE (<=), RCI = 7, LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<=@[7]test_var", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);
}

// OTT_LIST
BOOST_AUTO_TEST_CASE(fgn_bs_v1_lst){
    fgn::FilterManager fm;
    fgn::rule_param_t rdata;
    fgn::rule_param_t rdata_exp_res;
    // expected result
    rdata_exp_res.set_cstr( 1, "@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::OTT_LIST,   0, 1);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_cstr( 1, "test_list",     0, 4);

    // default operator
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = @{test_list}, OPERATOR = default (==)");
    int res = fm.prepare_match_line(&rdata, "@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // default operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = [6]@{test_list}, OPERATOR = default (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator
    rdata_exp_res.set_cstr( 1, "==@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = ==@{test_list}, OPERATOR = EQ (==)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "==@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // == operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]==@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_EQ,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = [6]==@{test_list}, OPERATOR = EQ (==), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]==@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator
    rdata_exp_res.set_cstr( 1, "!=@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 0,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = !=@{test_list}, OPERATOR = NE (!=)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "!=@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // != operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]!=@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_NE,     0, 3);
    rdata_exp_res.set_int ( 1, 6,               0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = [6]!=@{test_list}, OPERATOR = NE (!=), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]!=@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >> operator
    rdata_exp_res.set_cstr( 1, ">>@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_LST_ADD,     0, 3);
    rdata_exp_res.set_int ( 1, 0,                    0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = >>@{test_list}, OPERATOR = LST_ADD (>>)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, ">>@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // >> operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]>>@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_LST_ADD,     0, 3);
    rdata_exp_res.set_int ( 1, 6,                    0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = [6]>>@{test_list}, OPERATOR = LST_ADD (>>), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]>>@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // << operator
    rdata_exp_res.set_cstr( 1, "<<@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_LST_DEL,     0, 3);
    rdata_exp_res.set_int ( 1, 0,                    0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = <<@{test_list}, OPERATOR = LST_DEL (<<)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "<<@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // << operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]<<@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_LST_DEL,     0, 3);
    rdata_exp_res.set_int ( 1, 6,                    0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = [6]<<@{test_list}, OPERATOR = LST_DEL (<<), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]<<@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // -- operator
    rdata_exp_res.set_cstr( 1, "--@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_LST_CLR,     0, 3);
    rdata_exp_res.set_int ( 1, 0,                    0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = --@{test_list}, OPERATOR = LST_CLR (--)");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "--@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);

    // -- operator with LCI
    rdata_exp_res.set_cstr( 1, "[6]--@{test_list}");
    rdata_exp_res.set_int ( 1, fgn::ROT_LST_CLR,     0, 3);
    rdata_exp_res.set_int ( 1, 6,                    0, 2);
    BOOST_TEST_MESSAGE("Testing fgn::OTT_LIST OperandType: VALUE = [6]--@{test_list}, OPERATOR = LST_CLR (--), LCI = 6");
    rdata.clear_params();
    res = fm.prepare_match_line(&rdata, "[6]--@{test_list}", 1);
    BOOST_CHECK(res == 0);
    BOOST_TEST_MESSAGE("Generated rule:\n" << rdata);
    BOOST_TEST_MESSAGE("Expected rule:\n" << rdata_exp_res);
    BOOST_CHECK(rdata == rdata_exp_res);
}

BOOST_AUTO_TEST_SUITE_END()


#ifdef FGN_CPP14
// fgn basic matching syntax test suite v2
BOOST_AUTO_TEST_SUITE(fgn_basic_syntax_v2)

BOOST_AUTO_TEST_CASE(fgn_bs_v2){
    fgn::FilterManager fm;
    fgn::rule_param_t rdata;
    std::string test_var;
    // INT variants
    test_var = "10 1";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  67  ]   101";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "[  6 7 ]   101";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  67 }   101";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  67    101";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  67 ]   10a1";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  67 ]   10 1";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  67 ]   101 aaa";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  67 ]   101aaa";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "  67 ]   101";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    // STRING variants
    test_var = "[ 67  ]   test string";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "[ 67     test string";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[ 67 }    test string";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[ 67 ]    55test string";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[ 6 7 ]    '55test string'";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[ 67 ]    '55test string'";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    // VARIABLE variants
    test_var = "@    map.imsi";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "@    map imsi";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@    78  ]  map.imsi";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@    78  ]  map.imsi";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@    78   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@  [ 123  ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "@  [   ]  map.imsi  ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  678 ]  @  [ 123  ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "[  678 ]  @  [   ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[]  @  [   ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  678 ]  @ = [ 123  ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  678 ]  @ map.imsi [ 123  ]  78   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  678 ]  @ [ +  ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "[  678 ]  @ [ ++  ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  678 ]  @ [ + +  ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "[  -> ]  @ [ <-  ]  map.imsi   ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    // LIST variants
    test_var = "@ {  test_list  }";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "@ {  test list  }";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@   test_list  }";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@ {  test_list  ";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@ {  test_list  } aa";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@ {  5678  }";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 0);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == true);

    test_var = "@ [ 67  ]  {  5678  }";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

    test_var = "@ [ ]  {  5678  }";
    BOOST_TEST_MESSAGE("Testing basic syntax v2 parser: VALUE = " << test_var);
    BOOST_CHECK(fm.prepare_match_line(&rdata, test_var.c_str(), 1) == 1);
    BOOST_CHECK(fm.field_valid(test_var.c_str(), false) == false);

}

BOOST_AUTO_TEST_SUITE_END()
#endif

