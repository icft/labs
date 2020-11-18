#include "pch.h"
#include "../Testing_3/Scheme.h"

TEST(Scheme, TestFreeConstructor) {
	Scheme sc;
	ASSERT_EQ(sc.get_amount(), 0);
}

TEST(Scheme, TestInpOutConstructor) {
	Scheme sc(3, 4);
	ASSERT_EQ(sc.get_amount(), 7);
	struct clem* a = sc.get_arr();
	for (int i = 0; i < 3; i++) {
		ASSERT_EQ(a[i].number, i + 1);
		ASSERT_EQ(a[i].type, 0);
		ASSERT_EQ(a[i].count, 0);
		ASSERT_EQ(a[i].signal, 'x');
	}
	for (int i = 3; i < sc.get_amount(); i++) {
		ASSERT_EQ(a[i].number, i + 1);
		ASSERT_EQ(a[i].type, 1);
		ASSERT_EQ(a[i].count, 0);
		ASSERT_EQ(a[i].signal, 'x');
	}
	EXPECT_THROW(Scheme(6, 7), std::exception);
}

TEST(Scheme, TestArrConstructor) {
	struct clem a[3];
	a[0] = { 45, 1, 1, 'x' };
	a[1] = { 34, 0, 1, '0' };
	a[2] = { 56, 1, 3, '1' };
	Scheme sc(a, 3);
	struct clem* arr = sc.get_arr();
	ASSERT_EQ(arr[0].number, 45);
	ASSERT_EQ(arr[0].type, 1);
	ASSERT_EQ(arr[0].count, 1);
	ASSERT_EQ(arr[0].signal, 'x');
	ASSERT_EQ(arr[1].number, 34);
	ASSERT_EQ(arr[1].type, 0);
	ASSERT_EQ(arr[1].count, 1);
	ASSERT_EQ(arr[1].signal, '0');
	ASSERT_EQ(arr[2].number, 56);
	ASSERT_EQ(arr[2].type, 1);
	ASSERT_EQ(arr[2].count, 3);
	ASSERT_EQ(arr[2].signal, '1');
	struct clem* s = nullptr;
	EXPECT_THROW(Scheme(s, 5), std::exception);
	EXPECT_THROW(Scheme(a, 15), std::exception);
}

TEST(Scheme, TestFind) {
	struct clem a[3];
	a[0] = { 45, 1, 1, 'x' };
	a[1] = { 34, 0, 1, '0' };
	a[2] = { 56, 1, 3, '1' };
	Scheme sc(a, 3);
	ASSERT_EQ(sc.find(45), 0);
	ASSERT_EQ(sc.find(56), 2);
	ASSERT_NE(sc.find(34), 0);
	ASSERT_EQ(sc.find(1), -1);
	ASSERT_EQ(sc.find(909), -1);
	ASSERT_EQ(sc.find(0), -1);
}

TEST(Scheme, TestFunctor) {
	struct clem a[3];
	a[0] = { 45, 1, 1, 'x' };
	a[1] = { 34, 0, 1, '0' };
	a[2] = { 56, 1, 3, '1' };
	Scheme sc(a, 3);
	sc(45, {45, 1, 3 ,'1'});
	sc(34, { 34, 0, 1 ,'x' });
	EXPECT_THROW(sc(45, { 45, 1, 1, 'f' }), std::exception);
	EXPECT_THROW(sc(45, { 45, 5, 1, '0' }), std::exception);
	EXPECT_THROW(sc(45, { 45, 1, 45, '1' }), std::exception);
	struct clem* arr = sc.get_arr();
	ASSERT_EQ(arr[0].type, 1);
	ASSERT_EQ(arr[0].count, 3);
	ASSERT_EQ(arr[0].signal, '1');
}

TEST(Scheme, TestIndexOperator) {
	struct clem a[3];
	a[0] = { 45, 1, 1, 'x' };
	a[1] = { 34, 0, 1, '0' };
	a[2] = { 56, 1, 3, '1' };
	Scheme sc(a, 3);
	struct clem d = sc[34];
	ASSERT_EQ(d.number, 34);
	ASSERT_EQ(d.type, 0);
	ASSERT_EQ(d.count, 1);
	ASSERT_EQ(d.signal, '0');
	d = sc[56];
	ASSERT_EQ(d.number, 56);
	ASSERT_EQ(d.type, 1);
	ASSERT_EQ(d.count, 3);
	ASSERT_EQ(d.signal, '1');
	EXPECT_THROW(d = sc[5], std::exception);
}

TEST(Scheme, TestAdditionOverAssignmentOperator) {
	struct clem a[3];
	a[0] = { 45, 1, 1, 'x' };
	a[1] = { 34, 0, 1, '0' };
	a[2] = { 56, 1, 3, '1' };
	Scheme sc(a, 3);
	struct clem b[2];
	b[0] = { 1, 1, 3, '1' };
	b[1] = { 2, 0, 0, 'x' };
	Scheme sc1(b, 2);
	sc += sc1;
	ASSERT_EQ(sc.get_amount(), 5);
	struct clem* arr = sc.get_arr();
	ASSERT_EQ(arr[3].number, 1);
	ASSERT_EQ(arr[3].type, 1);
	ASSERT_EQ(arr[3].count, 3);
	ASSERT_EQ(arr[3].signal, '1');
	ASSERT_EQ(arr[4].number, 2);
	ASSERT_EQ(arr[4].type, 0);
	ASSERT_EQ(arr[4].count, 0);
	ASSERT_EQ(arr[4].signal, 'x');
	struct clem c[5];
	c[0] = { 1, 1, 3, '1' };
	c[1] = { 2, 0, 0, 'x' };
	c[2] = { 1, 1, 3, '1' };
	c[3] = { 2, 0, 0, 'x' };
	c[4] = { 1, 1, 3, '1' };
	Scheme sc2(c, 4);
	EXPECT_THROW(sc += sc2, std::exception);
}

TEST(Scheme, TestAdditionOperator) {
	struct clem a[1];
	a[0] = { 45, 1, 1, 'x' };
	Scheme sc(a, 1);
	struct clem b[2];
	b[0] = { 1, 1, 3, '1' };
	b[1] = { 2, 0, 0, 'x' };
	Scheme sc1(b, 2);
	Scheme s = sc + sc1;
	struct clem* arr = s.get_arr();
	ASSERT_EQ(arr[0].number, 45);
	ASSERT_EQ(arr[0].type, 1);
	ASSERT_EQ(arr[0].count, 1);
	ASSERT_EQ(arr[0].signal, 'x');
	ASSERT_EQ(arr[1].number, 1);
	ASSERT_EQ(arr[1].type, 1);
	ASSERT_EQ(arr[1].count, 3);
	ASSERT_EQ(arr[1].signal, '1');
	ASSERT_EQ(arr[2].number, 2);
	ASSERT_EQ(arr[2].type, 0);
	ASSERT_EQ(arr[2].count, 0);
	ASSERT_EQ(arr[2].signal, 'x');
	struct clem c[7];
	c[0] = { 1, 1, 3, '1' };
	c[1] = { 2, 0, 0, 'x' };
	c[2] = { 1, 1, 3, '1' };
	c[3] = { 2, 0, 0, 'x' };
	c[4] = { 1, 1, 3, '1' };
	c[5] = { 1, 1, 3, '1' };
	c[6] = { 1, 1, 3, '1' };
	Scheme sc2(c, 7);
	EXPECT_THROW(Scheme s2 = sc + sc2, std::exception);
}


TEST(Scheme, TestAddClem) {
	struct clem b[2];
	b[0] = { 1, 1, 1, '1' };
	b[1] = { 2, 0, 1, 'x' };
	Scheme sc(b, 2);
	sc.add_clem_connection(1, 3);
	struct clem* arr = sc.get_arr();
	ASSERT_EQ(arr[0].count, 3);
	ASSERT_EQ(arr[1].count, 1);
	EXPECT_THROW(sc.add_clem_connection(1, 100000), std::exception);
	EXPECT_THROW(sc.add_clem_connection(2, 2), std::exception);
	EXPECT_THROW(sc.add_clem_connection(156, 1), std::exception);
}

TEST(Scheme, TestReduceClem) {
	struct clem b[2];
	b[0] = { 1, 1, 1, '1' };
	b[1] = { 2, 0, 1, 'x' };
	Scheme sc(b, 2);
	sc.reduce_clem_connection(2, 0);
	struct clem* arr = sc.get_arr();
	ASSERT_EQ(arr[0].count, 1);
	ASSERT_EQ(arr[1].count, 0);
	EXPECT_THROW(sc.reduce_clem_connection(1, 100000), std::exception);
	EXPECT_THROW(sc.reduce_clem_connection(2, 2), std::exception);
	EXPECT_THROW(sc.reduce_clem_connection(1, 3), std::exception);
	EXPECT_THROW(sc.reduce_clem_connection(0, 0), std::exception);
	EXPECT_THROW(sc.reduce_clem_connection(156, 1), std::exception);
}

TEST(Scheme, TestStream1) {
	std::stringstream s;
	struct clem d;
	s << 23 << " " << 1 << " "<<  1 << " " << "1";
	struct clem b[1];
	b[0] = { 23, 1, 0, 'x' };
	Scheme sc(b, 1);
	s >> sc;
	struct clem* arr = sc.get_arr();
	ASSERT_EQ(arr[0].number, 23);
	ASSERT_EQ(arr[0].type, 1);
	ASSERT_EQ(arr[0].count, 1);
	ASSERT_EQ(arr[0].signal, '1');
	s.clear();
	s << 23 << " " << 1 << " " << 4 << " " << "1";
	s >> sc;
	if (s.fail())
		SUCCEED();
	else
		FAIL();
	s.clear();
	s << 23 << " " << 13 << " " << 4 << " " << "1";
	s >> sc;
	if (s.fail())
		SUCCEED();
	else
		FAIL();
	s.clear();
	s << 23 << " " << 1 << " " << 4 << " " << "3";
	s >> sc;
	if (s.fail())
		SUCCEED();
	else
		FAIL();
	s.clear();
	s << 23 << " " << 0 << " " << 2 << " " << "1";
	s >> sc;
	if (s.fail())
		SUCCEED();
	else
		FAIL();
}
