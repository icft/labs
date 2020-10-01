#include "pch.h"
#include "../lab2/nephroid.h"
#define _USE_MATH_DEFINES 
#include <math.h>

TEST(nephroid, testConstructor) {
	nephroid n(5);
	ASSERT_EQ(n.Get_rad(), 5);
	nephroid n1;
	ASSERT_EQ(n1.Get_rad(), 0);
	nephroid n2(5);
	n2.radius(20);
	ASSERT_EQ(n2.Get_rad(), 20);
}

TEST(nephroid, testGet_L) {
	nephroid n(5);
	ASSERT_NEAR(n.Get_L(), 120, nephroid::ABS_ERROR);
}

TEST(nephroid, testGet_S) {
	nephroid n(5);
	ASSERT_NEAR(n.Get_S(), 942.47779607693792, nephroid::ABS_ERROR);
}

TEST(nephoroid, testGet_R) {
	nephroid n(5);
	ASSERT_NEAR(n.Get_R(M_PI), 0, nephroid::ABS_ERROR);
	ASSERT_NEAR(n.Get_R(M_PI / 2), 20, nephroid::ABS_ERROR);
	ASSERT_NEAR(n.Get_R(M_PI * 3 / 2), -20, nephroid::ABS_ERROR);
	ASSERT_NEAR(n.Get_R(M_PI / 6), 10, nephroid::ABS_ERROR);
}

TEST(nephroid, testGet_x) {
	nephroid n(5);
	ASSERT_NEAR(n.Get_x(M_PI), -10, nephroid::ABS_ERROR);
	ASSERT_NEAR(n.Get_x(M_PI / 2), 0, nephroid::ABS_ERROR);
	ASSERT_NEAR(n.Get_x(M_PI / 3), 12.5, nephroid::ABS_ERROR);
}

TEST(nephroid, testGet_y) {
	nephroid n(5);
	ASSERT_NEAR(n.Get_y(M_PI), 0, nephroid::ABS_ERROR);
	ASSERT_NEAR(n.Get_y(M_PI / 2), 20, nephroid::ABS_ERROR);
	ASSERT_NEAR(n.Get_y(M_PI / 6), 2.5, nephroid::ABS_ERROR);
}

TEST(nephroid, test_radius) {
	nephroid n(5);
	n.radius(20);
	ASSERT_NEAR(n.Get_rad(), 20, nephroid::ABS_ERROR);
	n.radius(41);
	ASSERT_NEAR(n.Get_rad(), 41, nephroid::ABS_ERROR);
}
