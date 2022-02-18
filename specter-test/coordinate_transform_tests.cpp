#include "pch.h"
#include "../src/coordinate_system.hpp"

TEST(transform, coordinate_system) {

	specter::vec3f surfaceNormal00(1.f, 0.f, 0.f);
	specter::vec3f surfaceNormal01(0.f, 1.f, 0.f);
	specter::vec3f surfaceNormal02(0.f, 0.f, 1.f);
	
	specter::CoordinateSystem frame00(surfaceNormal00);
	specter::CoordinateSystem frame01(surfaceNormal01);
	specter::CoordinateSystem frame02(surfaceNormal02);
	
	EXPECT_TRUE(frame00.xaxis == specter::vec3f(0.f, -1.f, 0.f));
	EXPECT_TRUE(frame00.yaxis == specter::vec3f(0.f, 0.f, -1.f));
	EXPECT_TRUE(frame00.zaxis == specter::vec3f(1.f, 0.f, 0.f));

	EXPECT_TRUE(frame01.xaxis == specter::vec3f(1.f, 0.f, 0.f));
	EXPECT_TRUE(frame01.yaxis == specter::vec3f(0.f, 0.f, -1.f));
	EXPECT_TRUE(frame01.zaxis == specter::vec3f(0.f, 1.f, 0.f));

	EXPECT_TRUE(frame02.xaxis == specter::vec3f(1.f, 0.f, 0.f));
	EXPECT_TRUE(frame02.yaxis == specter::vec3f(0.f, 1.f, 0.f));
	EXPECT_TRUE(frame02.zaxis == specter::vec3f(0.f, 0.f, 1.f));

	specter::vec3f v00(0.f, 0.5f, 1.f);
	auto v00local = frame00.toLocal(v00);
	std::cout << "v00local: " << v00local << '\n';
	std::cout << "v00World: " << frame00.toWorld(v00local) << '\n';

}