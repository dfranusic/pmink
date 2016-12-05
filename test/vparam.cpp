#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <pmink_utils.h>
#include <fstream>

// variant param tests
BOOST_AUTO_TEST_SUITE(vparam_test_suite)

// INT
BOOST_AUTO_TEST_CASE(vp_test_int){
    BOOST_TEST_MESSAGE("Testing DPT_INT VariantParamType: ID = 1, VALUE = 100");
    pmink_utils::PooledVPMap<uint32_t> m;
    m.set_int(1, 100);
    BOOST_TEST_MESSAGE("Map:\n" << m); 
    BOOST_CHECK(m.get_param(1) != NULL);
    BOOST_CHECK((int)*m.get_param(1) == 100);
}

// STRING
BOOST_AUTO_TEST_CASE(vp_test_string){
    BOOST_TEST_MESSAGE("Testing DPT_STRING VariantParamType: ID = 1, VALUE = \"test string\"");
    pmink_utils::PooledVPMap<uint32_t> m;
    m.set_cstr(1, "test string");
    BOOST_TEST_MESSAGE("Map:\n" << m); 
    BOOST_CHECK(m.get_param(1) != NULL);
    BOOST_CHECK(strcmp((char*)*m.get_param(1), "test string") == 0);
}

// DOUBLE
BOOST_AUTO_TEST_CASE(vp_test_double){
    BOOST_TEST_MESSAGE("Testing DPT_DOUBLE VariantParamType: ID = 1, VALUE = 1.256");
    pmink_utils::PooledVPMap<uint32_t> m;
    m.set_double(1, 1.256);
    BOOST_TEST_MESSAGE("Map:\n" << m); 
    BOOST_CHECK(m.get_param(1) != NULL);
    BOOST_CHECK((double)*m.get_param(1) == 1.256);
}

// CHAR
BOOST_AUTO_TEST_CASE(vp_test_char){
    BOOST_TEST_MESSAGE("Testing DPT_CHAR VariantParamType: ID = 1, VALUE = 'A'");
    pmink_utils::PooledVPMap<uint32_t> m;
    m.set_char(1, 'A');
    BOOST_TEST_MESSAGE("Map:\n" << m); 
    BOOST_CHECK(m.get_param(1) != NULL);
    BOOST_CHECK((char)*m.get_param(1) == 'A');
}

// BOOL
BOOST_AUTO_TEST_CASE(vp_test_bool){
    BOOST_TEST_MESSAGE("Testing DPT_BOOL VariantParamType: ID = 1, VALUE = true");
    pmink_utils::PooledVPMap<uint32_t> m;
    m.set_bool(1, true);
    BOOST_TEST_MESSAGE("Map:\n" << m); 
    BOOST_CHECK(m.get_param(1) != NULL);
    BOOST_CHECK((bool)*m.get_param(1) == true);
}

// OCTETS
BOOST_AUTO_TEST_CASE(vp_test_octets){
    BOOST_TEST_MESSAGE("Testing DPT_OCTETS VariantParamType: ID = 1, VALUE = {0x10, 0x20, 0x30, 0x40, 0x50}");
    unsigned char test_buff[] = {0x10, 0x20, 0x30, 0x40, 0x50};
    pmink_utils::PooledVPMap<uint32_t> m;
    m.set_octets(1, test_buff, sizeof(test_buff));
    BOOST_TEST_MESSAGE("Map:\n" << m); 
    BOOST_CHECK(m.get_param(1) != NULL);
    BOOST_CHECK(sizeof(test_buff) == m.get_param(1)->get_size());
    BOOST_CHECK(memcmp((unsigned char*)*m.get_param(1), test_buff, sizeof(test_buff)) == 0);
}

// POINTER
BOOST_AUTO_TEST_CASE(vp_test_pointer){
    pmink_utils::PooledVPMap<uint32_t> m;
    int test_num = 10;
    BOOST_TEST_MESSAGE("Testing DPT_POINTER VariantParamType: ID = 1, VALUE = " << &test_num);
    m.set_pointer(1, &test_num);
    BOOST_TEST_MESSAGE("Map:\n" << m); 
    BOOST_CHECK(m.get_param(1) != NULL);
    BOOST_CHECK((void*)*m.get_param(1) == &test_num);
}

// vparam map equality
BOOST_AUTO_TEST_CASE(vp_test_equality){
    BOOST_TEST_MESSAGE("Testing VariantParamMap equality operator (==)");
    pmink_utils::PooledVPMap<uint32_t> m1;
    pmink_utils::PooledVPMap<uint32_t> m2;
    // map 1
    m1.set_int(1, 10);
    m1.set_double(2, 1.5);
    m1.set_cstr(3, "test string");
    // map 2
    m2.set_int(1, 10);
    m2.set_double(2, 1.5);
    m2.set_cstr(3, "test string");
    // check equality
    BOOST_TEST_MESSAGE("Map 1:\n" << m1); 
    BOOST_TEST_MESSAGE("Map 2:\n" << m2); 
    BOOST_CHECK(m1 == m2);
}

// vparam map inequality
BOOST_AUTO_TEST_CASE(vp_test_inequality){
    BOOST_TEST_MESSAGE("Testing VariantParamMap inequality operator (!=)");
    pmink_utils::PooledVPMap<uint32_t> m1;
    pmink_utils::PooledVPMap<uint32_t> m2;
    // map 1
    m1.set_int(1, 10);
    m1.set_double(2, 1.5);
    m1.set_cstr(3, "test string");
    // map 2
    m2.set_int(1, 10);
    m2.set_double(2, 1.5);
    m2.set_cstr(3, "test string!");
    // check equality
    BOOST_TEST_MESSAGE("Map 1:\n" << m1); 
    BOOST_TEST_MESSAGE("Map 2:\n" << m2); 
    BOOST_CHECK(m1 != m2);
}

// vparam assignment operator
BOOST_AUTO_TEST_CASE(vp_test_assign){
    BOOST_TEST_MESSAGE("Testing VariantParamMap assignment operator (=)");
    pmink_utils::PooledVPMap<uint32_t> m1;
    pmink_utils::PooledVPMap<uint32_t> m2;
    // map 1
    m1.set_int(1, 10);
    m1.set_double(2, 1.5);
    m1.set_cstr(3, "test string");
    BOOST_TEST_MESSAGE("Map 1:\n" << m1); 
    BOOST_TEST_MESSAGE("Map 2:\n" << m2); 
    // map 2
    BOOST_CHECK(m1 != m2);
    BOOST_TEST_MESSAGE("Map 1 = Map2\n"); 
    m2 = m1;
    // check equality
    BOOST_TEST_MESSAGE("Map 1:\n" << m1); 
    BOOST_TEST_MESSAGE("Map 2:\n" << m2); 
    BOOST_CHECK(m1 == m2);
}

// vparam copy constructor
BOOST_AUTO_TEST_CASE(vp_test_copyctor){
    BOOST_TEST_MESSAGE("Testing VariantParamMap copy constructor");
    pmink_utils::PooledVPMap<uint32_t> m1;
    // map 1
    m1.set_int(1, 10);
    m1.set_double(2, 1.5);
    m1.set_cstr(3, "test string");
    // map 2
    pmink_utils::PooledVPMap<uint32_t> m2(m1);
    // check equality
    BOOST_TEST_MESSAGE("Map 1:\n" << m1); 
    BOOST_TEST_MESSAGE("Map 2:\n" << m2); 
    BOOST_CHECK(m1 == m2);
    BOOST_CHECK(&m1 != &m2);
}

// vparam clear params
BOOST_AUTO_TEST_CASE(vp_test_clear){
    BOOST_TEST_MESSAGE("Testing VariantParamMap clear");
    pmink_utils::PooledVPMap<uint32_t> m1;
    // map 1
    m1.set_int(1, 10);
    m1.set_double(2, 1.5);
    m1.set_cstr(3, "test string");
    // check equality
    BOOST_TEST_MESSAGE("Map:\n" << m1); 
    BOOST_CHECK(m1.get_begin() != m1.get_end());
    BOOST_TEST_MESSAGE("Calling clear_params()...");
    m1.clear_params();
    BOOST_TEST_MESSAGE("Map:\n" << m1); 
    BOOST_CHECK(m1.get_begin() == m1.get_end());
}

// vparam erase param
BOOST_AUTO_TEST_CASE(vp_test_erase){
    BOOST_TEST_MESSAGE("Testing VariantParamMap erase_param");
    pmink_utils::PooledVPMap<uint32_t> m1;
    // map 1
    m1.set_int(1, 10);
    m1.set_double(2, 1.5);
    m1.set_cstr(3, "test string");
    // check equality
    BOOST_TEST_MESSAGE("Map:\n" << m1); 
    BOOST_CHECK(m1.get_param(2) != NULL);
    BOOST_TEST_MESSAGE("Calling erase_param(2)...");
    m1.erase_param(2);
    BOOST_TEST_MESSAGE("Map:\n" << m1); 
    BOOST_CHECK(m1.get_param(2) == NULL);
}

// vparam find and set (two maps)
BOOST_AUTO_TEST_CASE(vp_find_set){
    BOOST_TEST_MESSAGE("Testing VariantParamMap find/set with two maps");
    pmink_utils::PooledVPMap<uint32_t> m1;
    pmink_utils::PooledVPMap<uint32_t> m2;
    // map 1
    m1.set_int(1, 10);
    m1.set_double(2, 1.5);
    m1.set_cstr(3, "test string");
    // map 2
    m2.set_int(1, 20);
    m2.set_double(2, 2.5);
    m2.set_cstr(3, "another string");
    // check equality
    BOOST_TEST_MESSAGE("Map 1:\n" << m1); 
    BOOST_TEST_MESSAGE("Map 2:\n" << m2); 
    BOOST_CHECK(m1 != m2);
    BOOST_TEST_MESSAGE("Setting vparam in Map 2(ID = 9999) using value from Map 1(ID = 1)");
    m2.set(m1, pmink_utils::ParamTuple<>(1), pmink_utils::ParamTuple<>(9999));
    BOOST_TEST_MESSAGE("Map 1:\n" << m1); 
    BOOST_TEST_MESSAGE("Map 2:\n" << m2); 
    BOOST_CHECK(m1.get_param(1) != NULL);
    BOOST_CHECK(m2.get_param(9999) != NULL);
    BOOST_CHECK(*m1.get_param(1) == *m2.get_param(9999));
}


BOOST_AUTO_TEST_SUITE_END()
