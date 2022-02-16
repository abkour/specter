#include "pch.h"
#include "../src/aabb.hpp"

//
// AxisAlignedBoundingBox
TEST(constructor, aabb) {
	specter::AxisAlignedBoundingBox aabb00;
	ASSERT_TRUE(aabb00.min == aabb00.max);
	ASSERT_TRUE(aabb00.min == specter::vec3f(0.f));
	ASSERT_TRUE(aabb00.max == specter::vec3f(0.f));

	specter::AxisAlignedBoundingBox aabb01(specter::vec3f(-1.f), specter::vec3f(1.f));
	ASSERT_TRUE(aabb01.min == specter::vec3f(-1.f));
	ASSERT_TRUE(aabb01.max == specter::vec3f(1.f));

	specter::AxisAlignedBoundingBox aabb02(aabb01);
	ASSERT_TRUE(aabb02.min == specter::vec3f(-1.f));
	ASSERT_TRUE(aabb02.max == specter::vec3f(1.f));

	specter::AxisAlignedBoundingBox aabb03 = aabb02;
	ASSERT_TRUE(aabb03.min == specter::vec3f(-1.f));
	ASSERT_TRUE(aabb03.max == specter::vec3f(1.f));

	ASSERT_TRUE(aabb02 == aabb03);
	ASSERT_TRUE(aabb02 != aabb00);
}

TEST(assertions, aabb) {
	specter::AxisAlignedBoundingBox aabb00;
	ASSERT_TRUE(aabb00.isValid() == false);
	ASSERT_TRUE(aabb00.isCollapsed() == true);
}

TEST(intersections, aabb) {
	specter::AxisAlignedBoundingBox aabb00(specter::vec3f(0.f), specter::vec3f(1.f));
	specter::Ray ray00(specter::vec3f(0.f, 0.f, -1.f), specter::vec3f(0.f, 0.f, 1.f));
	float near, far;
	ASSERT_TRUE(aabb00.rayIntersect(ray00, near, far) == true);
	ASSERT_TRUE(near == 1.f);
	ASSERT_TRUE(far == 2.f);

	specter::Ray ray01(specter::vec3f(0.f, -1.f, 0.f), specter::vec3f(0.f, 1.f, 0.f));
	ASSERT_TRUE(aabb00.rayIntersect(ray01, near, far) == true);
	ASSERT_TRUE(near == 1.f);
	ASSERT_TRUE(far == 2.f);

	specter::Ray ray02(specter::vec3f(-1.f, 0.f, 0.f), specter::vec3f(1.f, 0.f, 0.f));
	ASSERT_TRUE(aabb00.rayIntersect(ray02, near, far) == true);
	ASSERT_TRUE(near == 1.f);
	ASSERT_TRUE(far == 2.f);

	// Rays going in the opposite direction of the AABB should not collide with the AABB
	specter::Ray ray03(specter::vec3f(0.f, 0.f, 3.f), specter::vec3f(0.f, 0.f, 1.f));
	EXPECT_TRUE(aabb00.rayIntersect(ray03, near, far) == false);
}

TEST(miscellaneous, aabb) {
	specter::vec3f v0(0.f);
	specter::vec3f v1(0.5f);
	specter::AxisAlignedBoundingBox aabb00(specter::vec3f(0.f), specter::vec3f(1.f));
	ASSERT_TRUE(aabb00.contains(v0) == false);
	ASSERT_TRUE(aabb00.contains(v1) == true);
	ASSERT_TRUE(aabb00.containsEdgeInclusive(v0) == true);
	ASSERT_TRUE(aabb00.containsEdgeInclusive(v1) == true);

	ASSERT_TRUE(aabb00.overlaps(aabb00) == true);
	ASSERT_TRUE(aabb00.overlapsEdgeInclusive(aabb00) == true);

	specter::AxisAlignedBoundingBox aabb01(specter::vec3f(-1.f, 0.f, 0.f), specter::vec3f(0.f));
	ASSERT_TRUE(aabb00.overlaps(aabb01) == false);
	ASSERT_TRUE(aabb00.overlapsEdgeInclusive(aabb01) == true);

	specter::AxisAlignedBoundingBox aabb02(specter::vec3f(0.f, -1.f, 0.f), specter::vec3f(0.f));
	ASSERT_TRUE(aabb00.overlaps(aabb02) == false);
	ASSERT_TRUE(aabb00.overlapsEdgeInclusive(aabb02) == true);

	specter::AxisAlignedBoundingBox aabb03(specter::vec3f(0.f, 0.f, -1.f), specter::vec3f(0.f));
	ASSERT_TRUE(aabb00.overlaps(aabb03) == false);
	ASSERT_TRUE(aabb00.overlapsEdgeInclusive(aabb03) == true);

	specter::AxisAlignedBoundingBox aabb04(specter::vec3f(0.f, 0.f, 0.2f), specter::vec3f(1.f));
	ASSERT_TRUE(aabb00.overlaps(aabb04) == true);
	ASSERT_TRUE(aabb00.overlapsEdgeInclusive(aabb04) == true);

	specter::AxisAlignedBoundingBox aabb05(specter::vec3f(0.f), specter::vec3f(2.f));
	ASSERT_TRUE(aabb05.volume() == 8.f);
	ASSERT_TRUE(aabb05.surfaceArea() == 24.f);
	ASSERT_TRUE(aabb05.center() == specter::vec3f(1.f));

	specter::AxisAlignedBoundingBox aabb06(specter::vec3f(-2.f), specter::vec3f(0.f));
	ASSERT_TRUE(aabb06.volume() == 8.f);
	ASSERT_TRUE(aabb06.surfaceArea() == 24.f);
	ASSERT_TRUE(aabb06.center() == specter::vec3f(-1.f));
}