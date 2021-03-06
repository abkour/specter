#include "pch.h"
#include "../src/mat3.hpp"
#include "../src/mat4.hpp"

//
// mat3
TEST(constructor, mat3) {
	specter::mat3i m00;
	EXPECT_TRUE(m00[0][0] == 0);
	EXPECT_TRUE(m00[1][1] == 0);
	EXPECT_TRUE(m00[2][2] == 0);

	specter::mat3i m01(1);
	EXPECT_TRUE(m01[0][0] == 1);
	EXPECT_TRUE(m01[1][1] == 1);
	EXPECT_TRUE(m01[2][2] == 1);

	specter::vec3i col0(1, 1, 1);
	specter::vec3i col1(2, 2, 2);
	specter::vec3i col2(3, 3, 3);
	specter::mat3i m02(col0, col1, col2);
	EXPECT_TRUE(m02[0][0] == col0[0]);
	EXPECT_TRUE(m02[0][1] == col1[0]);
	EXPECT_TRUE(m02[0][2] == col2[0]);
	EXPECT_TRUE(m02[1][0] == col0[1]);
	EXPECT_TRUE(m02[1][1] == col1[1]);
	EXPECT_TRUE(m02[1][2] == col2[1]);
	EXPECT_TRUE(m02[2][0] == col0[2]);
	EXPECT_TRUE(m02[2][1] == col1[2]);
	EXPECT_TRUE(m02[2][2] == col2[2]);

	specter::vec3i col3(1, 2, 3);
	specter::mat3i m03(col3);
	EXPECT_TRUE(m03[0][0] == col3[0]);
	EXPECT_TRUE(m03[0][1] == col3[0]);
	EXPECT_TRUE(m03[0][2] == col3[0]);
	EXPECT_TRUE(m03[1][0] == col3[1]);
	EXPECT_TRUE(m03[1][1] == col3[1]);
	EXPECT_TRUE(m03[1][2] == col3[1]);
	EXPECT_TRUE(m03[2][0] == col3[2]);
	EXPECT_TRUE(m03[2][1] == col3[2]);
	EXPECT_TRUE(m03[2][2] == col3[2]);

	specter::mat3i m04(m03);
	EXPECT_TRUE(m04[0][0] == m03[0][0]);
	EXPECT_TRUE(m04[0][1] == m03[0][1]);
	EXPECT_TRUE(m04[0][2] == m03[0][2]);
	EXPECT_TRUE(m04[1][0] == m03[1][0]);
	EXPECT_TRUE(m04[1][1] == m03[1][1]);
	EXPECT_TRUE(m04[1][2] == m03[1][2]);
	EXPECT_TRUE(m04[2][0] == m03[2][0]);
	EXPECT_TRUE(m04[2][1] == m03[2][1]);
	EXPECT_TRUE(m04[2][2] == m03[2][2]);

	specter::mat3i m05 = m04;
	EXPECT_TRUE(m05[0][0] == m04[0][0]);
	EXPECT_TRUE(m05[0][1] == m04[0][1]);
	EXPECT_TRUE(m05[0][2] == m04[0][2]);
	EXPECT_TRUE(m05[1][0] == m04[1][0]);
	EXPECT_TRUE(m05[1][1] == m04[1][1]);
	EXPECT_TRUE(m05[1][2] == m04[1][2]);
	EXPECT_TRUE(m05[2][0] == m04[2][0]);
	EXPECT_TRUE(m05[2][1] == m04[2][1]);
	EXPECT_TRUE(m05[2][2] == m04[2][2]);

	specter::mat3i m06(std::move(specter::mat3i(m05)));
	EXPECT_TRUE(m06[0][0] == m05[0][0]);
	EXPECT_TRUE(m06[0][1] == m05[0][1]);
	EXPECT_TRUE(m06[0][2] == m05[0][2]);
	EXPECT_TRUE(m06[1][0] == m05[1][0]);
	EXPECT_TRUE(m06[1][1] == m05[1][1]);
	EXPECT_TRUE(m06[1][2] == m05[1][2]);
	EXPECT_TRUE(m06[2][0] == m05[2][0]);
	EXPECT_TRUE(m06[2][1] == m05[2][1]);
	EXPECT_TRUE(m06[2][2] == m05[2][2]);

	specter::mat3i m07 = specter::mat3i(m06);
	EXPECT_TRUE(m07[0][0] == m06[0][0]);
	EXPECT_TRUE(m07[0][1] == m06[0][1]);
	EXPECT_TRUE(m07[0][2] == m06[0][2]);
	EXPECT_TRUE(m07[1][0] == m06[1][0]);
	EXPECT_TRUE(m07[1][1] == m06[1][1]);
	EXPECT_TRUE(m07[1][2] == m06[1][2]);
	EXPECT_TRUE(m07[2][0] == m06[2][0]);
	EXPECT_TRUE(m07[2][1] == m06[2][1]);
	EXPECT_TRUE(m07[2][2] == m06[2][2]);
}

TEST(comparison, mat3) {
	specter::vec3i col0(1, 1, 1);
	specter::vec3i col1(2, 2, 2);
	specter::vec3i col2(3, 3, 3);
	specter::mat3i m00(col0, col1, col2);
	specter::mat3i m01(col0, col1, col2);

	EXPECT_TRUE(m00 == m01);
	EXPECT_EQ(m00 != m01, false);
}

TEST(arithmetic, mat3) {
	specter::vec3i col0(1, 1, 1);
	specter::vec3i col1(2, 2, 2);
	specter::vec3i col2(3, 3, 3);
	specter::mat3i m00(col0, col1, col2);
	specter::mat3i m01(m00);
	
	specter::mat3i m02 = m00 + m01;
	EXPECT_TRUE(m02[0][0] == col0[0] + col0[0]);
	EXPECT_TRUE(m02[0][1] == col1[0] + col1[0]);
	EXPECT_TRUE(m02[0][2] == col2[0] + col2[0]);
	EXPECT_TRUE(m02[1][0] == col0[1] + col0[1]);
	EXPECT_TRUE(m02[1][1] == col1[1] + col1[1]);
	EXPECT_TRUE(m02[1][2] == col2[1] + col2[1]);
	EXPECT_TRUE(m02[2][0] == col0[2] + col0[2]);
	EXPECT_TRUE(m02[2][1] == col1[2] + col1[2]);
	EXPECT_TRUE(m02[2][2] == col2[2] + col2[2]);
	
	
	specter::mat3i m03 = m00 - m01;
	EXPECT_TRUE(m03[0][0] == m00[0][0] - m01[0][0]);
	EXPECT_TRUE(m03[0][1] == m00[0][1] - m01[0][1]);
	EXPECT_TRUE(m03[0][2] == m00[0][2] - m01[0][2]);
	EXPECT_TRUE(m03[1][0] == m00[1][0] - m01[1][0]);
	EXPECT_TRUE(m03[1][1] == m00[1][1] - m01[1][1]);
	EXPECT_TRUE(m03[1][2] == m00[1][2] - m01[1][2]);
	EXPECT_TRUE(m03[2][0] == m00[2][0] - m01[2][0]);
	EXPECT_TRUE(m03[2][1] == m00[2][1] - m01[2][1]);
	EXPECT_TRUE(m03[2][2] == m00[2][2] - m01[2][2]);

	specter::mat3i m04 = m00 * m00;
	// Results were calculated by hand
	EXPECT_TRUE(m04[0][0] == 6);
	EXPECT_TRUE(m04[0][1] == 12);
	EXPECT_TRUE(m04[0][2] == 18);
	EXPECT_TRUE(m04[1][0] == 6);
	EXPECT_TRUE(m04[1][1] == 12);
	EXPECT_TRUE(m04[1][2] == 18);
	EXPECT_TRUE(m04[2][0] == 6);
	EXPECT_TRUE(m04[2][1] == 12);
	EXPECT_TRUE(m04[2][2] == 18);

	specter::mat3i m05(m00);
	m05 += m00;
	EXPECT_TRUE(m05[0][0] == col0[0] + col0[0]);
	EXPECT_TRUE(m05[0][1] == col1[0] + col1[0]);
	EXPECT_TRUE(m05[0][2] == col2[0] + col2[0]);
	EXPECT_TRUE(m05[1][0] == col0[1] + col0[1]);
	EXPECT_TRUE(m05[1][1] == col1[1] + col1[1]);
	EXPECT_TRUE(m05[1][2] == col2[1] + col2[1]);
	EXPECT_TRUE(m05[2][0] == col0[2] + col0[2]);
	EXPECT_TRUE(m05[2][1] == col1[2] + col1[2]);
	EXPECT_TRUE(m05[2][2] == col2[2] + col2[2]);

	specter::mat3i m06(m01);
	m06 -= m00;
	EXPECT_TRUE(m06[0][0] == m01[0][0] - m00[0][0]);
	EXPECT_TRUE(m06[0][1] == m01[0][1] - m00[0][1]);
	EXPECT_TRUE(m06[0][2] == m01[0][2] - m00[0][2]);
	EXPECT_TRUE(m06[1][0] == m01[1][0] - m00[1][0]);
	EXPECT_TRUE(m06[1][1] == m01[1][1] - m00[1][1]);
	EXPECT_TRUE(m06[1][2] == m01[1][2] - m00[1][2]);
	EXPECT_TRUE(m06[2][0] == m01[2][0] - m00[2][0]);
	EXPECT_TRUE(m06[2][1] == m01[2][1] - m00[2][1]);
	EXPECT_TRUE(m06[2][2] == m01[2][2] - m00[2][2]);
	
	specter::mat3i m07(m00);
	m07 *= m00;
	EXPECT_TRUE(m07[0][0] == 6);
	EXPECT_TRUE(m07[0][1] == 12);
	EXPECT_TRUE(m07[0][2] == 18);
	EXPECT_TRUE(m07[1][0] == 6);
	EXPECT_TRUE(m07[1][1] == 12);
	EXPECT_TRUE(m07[1][2] == 18);
	EXPECT_TRUE(m07[2][0] == 6);
	EXPECT_TRUE(m07[2][1] == 12);
	EXPECT_TRUE(m07[2][2] == 18);

	specter::mat3i m08(m00 * 10);
	EXPECT_TRUE(m08[0][0] == m00[0][0] * 10);
	EXPECT_TRUE(m08[0][1] == m00[0][1] * 10);
	EXPECT_TRUE(m08[0][2] == m00[0][2] * 10);
	EXPECT_TRUE(m08[1][0] == m00[1][0] * 10);
	EXPECT_TRUE(m08[1][1] == m00[1][1] * 10);
	EXPECT_TRUE(m08[1][2] == m00[1][2] * 10);
	EXPECT_TRUE(m08[2][0] == m00[2][0] * 10);
	EXPECT_TRUE(m08[2][1] == m00[2][1] * 10);
	EXPECT_TRUE(m08[2][2] == m00[2][2] * 10);

	specter::mat3i m09(m08 / 10);
	EXPECT_TRUE(m09[0][0] == m00[0][0]);
	EXPECT_TRUE(m09[0][1] == m00[0][1]);
	EXPECT_TRUE(m09[0][2] == m00[0][2]);
	EXPECT_TRUE(m09[1][0] == m00[1][0]);
	EXPECT_TRUE(m09[1][1] == m00[1][1]);
	EXPECT_TRUE(m09[1][2] == m00[1][2]);
	EXPECT_TRUE(m09[2][0] == m00[2][0]);
	EXPECT_TRUE(m09[2][1] == m00[2][1]);
	EXPECT_TRUE(m09[2][2] == m00[2][2]);

	specter::mat3i m10(m00);
	m10 *= 10;
	EXPECT_TRUE(m10[0][0] == m00[0][0] * 10);
	EXPECT_TRUE(m10[0][1] == m00[0][1] * 10);
	EXPECT_TRUE(m10[0][2] == m00[0][2] * 10);
	EXPECT_TRUE(m10[1][0] == m00[1][0] * 10);
	EXPECT_TRUE(m10[1][1] == m00[1][1] * 10);
	EXPECT_TRUE(m10[1][2] == m00[1][2] * 10);
	EXPECT_TRUE(m10[2][0] == m00[2][0] * 10);
	EXPECT_TRUE(m10[2][1] == m00[2][1] * 10);
	EXPECT_TRUE(m10[2][2] == m00[2][2] * 10);

	specter::mat3i m11(m08);
	m11 /= 10;
	EXPECT_TRUE(m11[0][0] == m00[0][0]);
	EXPECT_TRUE(m11[0][1] == m00[0][1]);
	EXPECT_TRUE(m11[0][2] == m00[0][2]);
	EXPECT_TRUE(m11[1][0] == m00[1][0]);
	EXPECT_TRUE(m11[1][1] == m00[1][1]);
	EXPECT_TRUE(m11[1][2] == m00[1][2]);
	EXPECT_TRUE(m11[2][0] == m00[2][0]);
	EXPECT_TRUE(m11[2][1] == m00[2][1]);
	EXPECT_TRUE(m11[2][2] == m00[2][2]);
}

TEST(access, mat3) {
	specter::vec3i col0(1, 1, 1);
	specter::vec3i col1(2, 2, 2);
	specter::vec3i col2(3, 3, 3);
	specter::mat3i m00(col0, col1, col2);
	
	EXPECT_TRUE(m00[0][0] == 1);
	EXPECT_TRUE(m00[0][1] == 2);
	EXPECT_TRUE(m00[0][2] == 3);
	EXPECT_TRUE(m00[1][0] == 1);
	EXPECT_TRUE(m00[1][1] == 2);
	EXPECT_TRUE(m00[1][2] == 3);
	EXPECT_TRUE(m00[2][0] == 1);
	EXPECT_TRUE(m00[2][1] == 2);
	EXPECT_TRUE(m00[2][2] == 3);

	specter::mat3i m01(col0, col1, col2);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			m01[i][j] = i * 3 + j;
		}
	}
	EXPECT_TRUE(m01[0][0] == 0);
	EXPECT_TRUE(m01[0][1] == 1);
	EXPECT_TRUE(m01[0][2] == 2);
	EXPECT_TRUE(m01[1][0] == 3);
	EXPECT_TRUE(m01[1][1] == 4);
	EXPECT_TRUE(m01[1][2] == 5);
	EXPECT_TRUE(m01[2][0] == 6);
	EXPECT_TRUE(m01[2][1] == 7);
	EXPECT_TRUE(m01[2][2] == 8);
	
	specter::vec3i gcol0 = m00.col(0);
	specter::vec3i gcol1 = m00.col(1);
	specter::vec3i gcol2 = m00.col(2);

	EXPECT_TRUE(gcol0[0] == 1);
	EXPECT_TRUE(gcol0[1] == 1);
	EXPECT_TRUE(gcol0[2] == 1);
	EXPECT_TRUE(gcol1[0] == 2);
	EXPECT_TRUE(gcol1[1] == 2);
	EXPECT_TRUE(gcol1[2] == 2);
	EXPECT_TRUE(gcol2[0] == 3);
	EXPECT_TRUE(gcol2[1] == 3);
	EXPECT_TRUE(gcol2[2] == 3);

	specter::vec3i grow0 = m00.row(0);
	specter::vec3i grow1 = m00.row(1);
	specter::vec3i grow2 = m00.row(2);

	EXPECT_TRUE(grow0[0] == 1);
	EXPECT_TRUE(grow0[1] == 2);
	EXPECT_TRUE(grow0[2] == 3);
	EXPECT_TRUE(grow1[0] == 1);
	EXPECT_TRUE(grow1[1] == 2);
	EXPECT_TRUE(grow1[2] == 3);
	EXPECT_TRUE(grow2[0] == 1);
	EXPECT_TRUE(grow2[1] == 2);
	EXPECT_TRUE(grow2[2] == 3);
}

TEST(miscellaneous, mat3) {
	specter::mat3i id(1);
	EXPECT_TRUE(id[0][0] == 1);
	EXPECT_TRUE(id[1][1] == 1);
	EXPECT_TRUE(id[2][2] == 1);
	EXPECT_TRUE(id[0][1] == 0);
	EXPECT_TRUE(id[0][2] == 0);
	EXPECT_TRUE(id[1][0] == 0);
	EXPECT_TRUE(id[1][2] == 0);
	EXPECT_TRUE(id[2][0] == 0);
	EXPECT_TRUE(id[2][1] == 0);

	specter::vec3i col0(1, 1, 1);
	specter::vec3i col1(2, 2, 2);
	specter::vec3i col2(3, 3, 3);
	specter::mat3i m00(col0, col1, col2);

	EXPECT_TRUE(specter::trace(m00) == 6);

	// Calculated by hand
	EXPECT_TRUE(specter::determinant(m00) == 0);

	specter::mat3i m01 = specter::transpose(m00);
	EXPECT_TRUE(m00[0][0] == m01[0][0]);
	EXPECT_TRUE(m00[0][1] == m01[1][0]);
	EXPECT_TRUE(m00[0][2] == m01[2][0]);
	EXPECT_TRUE(m00[1][0] == m01[0][1]);
	EXPECT_TRUE(m00[1][1] == m01[1][1]);
	EXPECT_TRUE(m00[1][2] == m01[2][1]);
	EXPECT_TRUE(m00[2][0] == m01[0][2]);
	EXPECT_TRUE(m00[2][1] == m01[1][2]);
	EXPECT_TRUE(m00[2][2] == m01[2][2]);


	// Calculated by hand, easy case.
	specter::vec3f col4(1, 0, 0);
	specter::vec3f col5(0, 2, 0);
	specter::vec3f col6(0, 0, 3);
	specter::mat3f m02(col4, col5, col6);
	specter::mat3f m03 = specter::inverse(m02);
	EXPECT_TRUE(m03[0][0] == 1);
	EXPECT_TRUE(m03[0][1] == 0);
	EXPECT_TRUE(m03[0][2] == 0);
	EXPECT_TRUE(m03[1][0] == 0);
	EXPECT_TRUE(m03[1][1] == 1 / 2.f);
	EXPECT_TRUE(m03[1][2] == 0);
	EXPECT_TRUE(m03[2][0] == 0);
	EXPECT_TRUE(m03[2][1] == 0);
	EXPECT_TRUE(m03[2][2] == 1 / 3.f);

	// Using the fact that the A * A^-1 = I
	// Create random matrix
	specter::vec3f col7(0.2421, 0.8512, 0.961);
	specter::vec3f col8(0.0124, 0.1816516, 0.518712);
	specter::vec3f col9(0.7125, 0.523681, 0.2582);
	specter::mat3f m04(col7, col8, col9);
	specter::mat3f m05 = specter::inverse(m04);
	specter::mat3f m06 = m04 * m05;
	// Error propagation during inverse calculation results in significant deviation from correct result.
	EXPECT_TRUE(specter::isIdentityCorrected(m06));
}


//
// mat4
TEST(constructor, mat4) {
	specter::mat4i m00;
	EXPECT_TRUE(m00[0][0] == 0);
	EXPECT_TRUE(m00[1][1] == 0);
	EXPECT_TRUE(m00[2][2] == 0);
	EXPECT_TRUE(m00[3][3] == 0);

	specter::mat4i m01(1);
	EXPECT_TRUE(m01[0][0] == 1);
	EXPECT_TRUE(m01[1][1] == 1);
	EXPECT_TRUE(m01[2][2] == 1);
	EXPECT_TRUE(m01[3][3] == 1);

	specter::vec4i col0(1, 1, 1, 1);
	specter::vec4i col1(2, 2, 2, 2);
	specter::vec4i col2(3, 3, 3, 3);
	specter::vec4i col3(4, 4, 4, 4);
	specter::mat4i m02(col0, col1, col2, col3);
	EXPECT_TRUE(m02[0][0] == col0[0]);
	EXPECT_TRUE(m02[0][1] == col1[0]);
	EXPECT_TRUE(m02[0][2] == col2[0]);
	EXPECT_TRUE(m02[0][3] == col3[0]);
	EXPECT_TRUE(m02[1][0] == col0[1]);
	EXPECT_TRUE(m02[1][1] == col1[1]);
	EXPECT_TRUE(m02[1][2] == col2[1]);
	EXPECT_TRUE(m02[1][3] == col3[1]);
	EXPECT_TRUE(m02[2][0] == col0[2]);
	EXPECT_TRUE(m02[2][1] == col1[2]);
	EXPECT_TRUE(m02[2][2] == col2[2]);
	EXPECT_TRUE(m02[2][3] == col3[2]);
	EXPECT_TRUE(m02[3][0] == col0[3]);
	EXPECT_TRUE(m02[3][1] == col1[3]);
	EXPECT_TRUE(m02[3][2] == col2[3]);
	EXPECT_TRUE(m02[3][3] == col3[3]);

	specter::vec4i col4(1, 2, 3, 4);
	specter::mat4i m03(col4);
	EXPECT_TRUE(m03[0][0] == col4[0]);
	EXPECT_TRUE(m03[0][1] == col4[0]);
	EXPECT_TRUE(m03[0][2] == col4[0]);
	EXPECT_TRUE(m03[0][3] == col4[0]);
	EXPECT_TRUE(m03[1][0] == col4[1]);
	EXPECT_TRUE(m03[1][1] == col4[1]);
	EXPECT_TRUE(m03[1][2] == col4[1]);
	EXPECT_TRUE(m03[1][3] == col4[1]);
	EXPECT_TRUE(m03[2][0] == col4[2]);
	EXPECT_TRUE(m03[2][1] == col4[2]);
	EXPECT_TRUE(m03[2][2] == col4[2]);
	EXPECT_TRUE(m03[2][3] == col4[2]);
	EXPECT_TRUE(m03[3][0] == col4[3]);
	EXPECT_TRUE(m03[3][1] == col4[3]);
	EXPECT_TRUE(m03[3][2] == col4[3]);
	EXPECT_TRUE(m03[3][3] == col4[3]);

	specter::mat4i m04(m03);
	EXPECT_TRUE(m04[0][0] == m03[0][0]);
	EXPECT_TRUE(m04[0][1] == m03[0][1]);
	EXPECT_TRUE(m04[0][2] == m03[0][2]);
	EXPECT_TRUE(m04[0][3] == m03[0][3]);
	EXPECT_TRUE(m04[1][0] == m03[1][0]);
	EXPECT_TRUE(m04[1][1] == m03[1][1]);
	EXPECT_TRUE(m04[1][2] == m03[1][2]);
	EXPECT_TRUE(m04[1][3] == m03[1][3]);
	EXPECT_TRUE(m04[2][0] == m03[2][0]);
	EXPECT_TRUE(m04[2][1] == m03[2][1]);
	EXPECT_TRUE(m04[2][2] == m03[2][2]);
	EXPECT_TRUE(m04[2][3] == m03[2][3]);
	EXPECT_TRUE(m04[3][0] == m03[3][0]);
	EXPECT_TRUE(m04[3][1] == m03[3][1]);
	EXPECT_TRUE(m04[3][2] == m03[3][2]);
	EXPECT_TRUE(m04[3][3] == m03[3][3]);

	specter::mat4i m05 = m04;
	EXPECT_TRUE(m05[0][0] == m04[0][0]);
	EXPECT_TRUE(m05[0][1] == m04[0][1]);
	EXPECT_TRUE(m05[0][2] == m04[0][2]);
	EXPECT_TRUE(m05[0][3] == m04[0][3]);
	EXPECT_TRUE(m05[1][0] == m04[1][0]);
	EXPECT_TRUE(m05[1][1] == m04[1][1]);
	EXPECT_TRUE(m05[1][2] == m04[1][2]);
	EXPECT_TRUE(m05[1][3] == m04[1][3]);
	EXPECT_TRUE(m05[2][0] == m04[2][0]);
	EXPECT_TRUE(m05[2][1] == m04[2][1]);
	EXPECT_TRUE(m05[2][2] == m04[2][2]);
	EXPECT_TRUE(m05[2][3] == m04[2][3]);
	EXPECT_TRUE(m05[3][0] == m04[3][0]);
	EXPECT_TRUE(m05[3][1] == m04[3][1]);
	EXPECT_TRUE(m05[3][2] == m04[3][2]);
	EXPECT_TRUE(m05[3][3] == m04[3][3]);

	specter::mat4i m06(std::move(specter::mat4i(m05)));
	EXPECT_TRUE(m06[0][0] == m05[0][0]);
	EXPECT_TRUE(m06[0][1] == m05[0][1]);
	EXPECT_TRUE(m06[0][2] == m05[0][2]);
	EXPECT_TRUE(m06[0][3] == m05[0][3]);

	EXPECT_TRUE(m06[1][0] == m05[1][0]);
	EXPECT_TRUE(m06[1][1] == m05[1][1]);
	EXPECT_TRUE(m06[1][2] == m05[1][2]);
	EXPECT_TRUE(m06[1][3] == m05[1][3]);

	EXPECT_TRUE(m06[2][0] == m05[2][0]);
	EXPECT_TRUE(m06[2][1] == m05[2][1]);
	EXPECT_TRUE(m06[2][2] == m05[2][2]);
	EXPECT_TRUE(m06[2][3] == m05[2][3]);

	EXPECT_TRUE(m06[3][0] == m05[3][0]);
	EXPECT_TRUE(m06[3][1] == m05[3][1]);
	EXPECT_TRUE(m06[3][2] == m05[3][2]);
	EXPECT_TRUE(m06[3][3] == m05[3][3]);

	specter::mat4i m07 = specter::mat4i(m06);
	EXPECT_TRUE(m07[0][0] == m06[0][0]);
	EXPECT_TRUE(m07[0][1] == m06[0][1]);
	EXPECT_TRUE(m07[0][2] == m06[0][2]);
	EXPECT_TRUE(m07[0][3] == m06[0][3]);

	EXPECT_TRUE(m07[1][0] == m06[1][0]);
	EXPECT_TRUE(m07[1][1] == m06[1][1]);
	EXPECT_TRUE(m07[1][2] == m06[1][2]);
	EXPECT_TRUE(m07[1][3] == m06[1][3]);

	EXPECT_TRUE(m07[2][0] == m06[2][0]);
	EXPECT_TRUE(m07[2][1] == m06[2][1]);
	EXPECT_TRUE(m07[2][2] == m06[2][2]);
	EXPECT_TRUE(m07[2][3] == m06[2][3]);

	EXPECT_TRUE(m07[3][0] == m06[3][0]);
	EXPECT_TRUE(m07[3][1] == m06[3][1]);
	EXPECT_TRUE(m07[3][2] == m06[3][2]);
	EXPECT_TRUE(m07[3][3] == m06[3][3]);
}

TEST(comparison, mat4) {
	specter::vec4i col0(1, 1, 1, 1);
	specter::vec4i col1(2, 2, 2, 2);
	specter::vec4i col2(3, 3, 3, 3);
	specter::vec4i col3(4, 4, 4, 4);
	specter::mat4i m00(col0, col1, col2, col3);
	specter::mat4i m01(col0, col1, col2, col3);

	EXPECT_TRUE(m00 == m01);
	EXPECT_EQ(m00 != m01, false);
}

TEST(arithmetic, mat4) {
	specter::vec4i col0(1, 1, 1, 1);
	specter::vec4i col1(2, 2, 2, 2);
	specter::vec4i col2(3, 3, 3, 3);
	specter::vec4i col3(4, 4, 4, 4);
	specter::mat4i m00(col0, col1, col2, col3);
	specter::mat4i m01(m00);

	specter::mat4i m02 = m00 + m01;
	EXPECT_TRUE(m02[0][0] == col0[0] + col0[0]);
	EXPECT_TRUE(m02[0][1] == col1[0] + col1[0]);
	EXPECT_TRUE(m02[0][2] == col2[0] + col2[0]);
	EXPECT_TRUE(m02[0][3] == col3[0] + col3[0]);
	EXPECT_TRUE(m02[1][0] == col0[1] + col0[1]);
	EXPECT_TRUE(m02[1][1] == col1[1] + col1[1]);
	EXPECT_TRUE(m02[1][2] == col2[1] + col2[1]);
	EXPECT_TRUE(m02[1][3] == col3[1] + col3[1]);
	EXPECT_TRUE(m02[2][0] == col0[2] + col0[2]);
	EXPECT_TRUE(m02[2][1] == col1[2] + col1[2]);
	EXPECT_TRUE(m02[2][2] == col2[2] + col2[2]);
	EXPECT_TRUE(m02[2][3] == col3[2] + col3[2]);
	EXPECT_TRUE(m02[3][0] == col0[3] + col0[3]);
	EXPECT_TRUE(m02[3][1] == col1[3] + col1[3]);
	EXPECT_TRUE(m02[3][2] == col2[3] + col2[3]);
	EXPECT_TRUE(m02[3][3] == col3[3] + col3[3]);


	specter::mat4i m03 = m00 - m01;
	EXPECT_TRUE(m03[0][0] == m00[0][0] - m01[0][0]);
	EXPECT_TRUE(m03[0][1] == m00[0][1] - m01[0][1]);
	EXPECT_TRUE(m03[0][2] == m00[0][2] - m01[0][2]);
	EXPECT_TRUE(m03[0][3] == m00[0][3] - m01[0][3]);
	EXPECT_TRUE(m03[1][0] == m00[1][0] - m01[1][0]);
	EXPECT_TRUE(m03[1][1] == m00[1][1] - m01[1][1]);
	EXPECT_TRUE(m03[1][2] == m00[1][2] - m01[1][2]);
	EXPECT_TRUE(m03[1][3] == m00[1][3] - m01[1][3]);
	EXPECT_TRUE(m03[2][0] == m00[2][0] - m01[2][0]);
	EXPECT_TRUE(m03[2][1] == m00[2][1] - m01[2][1]);
	EXPECT_TRUE(m03[2][2] == m00[2][2] - m01[2][2]);
	EXPECT_TRUE(m03[2][3] == m00[2][3] - m01[2][3]);
	EXPECT_TRUE(m03[3][0] == m00[3][0] - m01[3][0]);
	EXPECT_TRUE(m03[3][1] == m00[3][1] - m01[3][1]);
	EXPECT_TRUE(m03[3][2] == m00[3][2] - m01[3][2]);
	EXPECT_TRUE(m03[3][3] == m00[3][3] - m01[3][3]);

	specter::mat4i m04 = m00 * m00;
	// Results were calculated by hand
	EXPECT_TRUE(m04[0][0] == 10);
	EXPECT_TRUE(m04[0][1] == 20);
	EXPECT_TRUE(m04[0][2] == 30);
	EXPECT_TRUE(m04[0][3] == 40);

	EXPECT_TRUE(m04[1][0] == 10);
	EXPECT_TRUE(m04[1][1] == 20);
	EXPECT_TRUE(m04[1][2] == 30);
	EXPECT_TRUE(m04[1][3] == 40);

	EXPECT_TRUE(m04[2][0] == 10);
	EXPECT_TRUE(m04[2][1] == 20);
	EXPECT_TRUE(m04[2][2] == 30);
	EXPECT_TRUE(m04[2][3] == 40);

	EXPECT_TRUE(m04[3][0] == 10);
	EXPECT_TRUE(m04[3][1] == 20);
	EXPECT_TRUE(m04[3][2] == 30);
	EXPECT_TRUE(m04[3][3] == 40);

	specter::mat4i m05(m00);
	m05 += m00;
	EXPECT_TRUE(m05[0][0] == col0[0] + col0[0]);
	EXPECT_TRUE(m05[0][1] == col1[0] + col1[0]);
	EXPECT_TRUE(m05[0][2] == col2[0] + col2[0]);
	EXPECT_TRUE(m05[0][3] == col3[0] + col3[0]);

	EXPECT_TRUE(m05[1][0] == col0[1] + col0[1]);
	EXPECT_TRUE(m05[1][1] == col1[1] + col1[1]);
	EXPECT_TRUE(m05[1][2] == col2[1] + col2[1]);
	EXPECT_TRUE(m05[1][3] == col3[1] + col3[1]);

	EXPECT_TRUE(m05[2][0] == col0[2] + col0[2]);
	EXPECT_TRUE(m05[2][1] == col1[2] + col1[2]);
	EXPECT_TRUE(m05[2][2] == col2[2] + col2[2]);
	EXPECT_TRUE(m05[2][3] == col3[2] + col3[2]);

	EXPECT_TRUE(m05[3][0] == col0[3] + col0[3]);
	EXPECT_TRUE(m05[3][1] == col1[3] + col1[3]);
	EXPECT_TRUE(m05[3][2] == col2[3] + col2[3]);
	EXPECT_TRUE(m05[3][3] == col3[3] + col3[3]);

	specter::mat4i m06(m01);
	m06 -= m00;
	EXPECT_TRUE(m06[0][0] == m01[0][0] - m00[0][0]);
	EXPECT_TRUE(m06[0][1] == m01[0][1] - m00[0][1]);
	EXPECT_TRUE(m06[0][2] == m01[0][2] - m00[0][2]);
	EXPECT_TRUE(m06[0][3] == m01[0][3] - m00[0][3]);
	EXPECT_TRUE(m06[1][0] == m01[1][0] - m00[1][0]);
	EXPECT_TRUE(m06[1][1] == m01[1][1] - m00[1][1]);
	EXPECT_TRUE(m06[1][2] == m01[1][2] - m00[1][2]);
	EXPECT_TRUE(m06[1][3] == m01[1][3] - m00[1][3]);
	EXPECT_TRUE(m06[2][0] == m01[2][0] - m00[2][0]);
	EXPECT_TRUE(m06[2][1] == m01[2][1] - m00[2][1]);
	EXPECT_TRUE(m06[2][2] == m01[2][2] - m00[2][2]);
	EXPECT_TRUE(m06[2][3] == m01[2][3] - m00[2][3]);
	EXPECT_TRUE(m06[3][0] == m01[3][0] - m00[3][0]);
	EXPECT_TRUE(m06[3][1] == m01[3][1] - m00[3][1]);
	EXPECT_TRUE(m06[3][2] == m01[3][2] - m00[3][2]);
	EXPECT_TRUE(m06[3][3] == m01[3][3] - m00[3][3]);

	specter::mat4i m07(m00);
	m07 *= m00;
	EXPECT_TRUE(m07[0][0] == 10);
	EXPECT_TRUE(m07[0][1] == 20);
	EXPECT_TRUE(m07[0][2] == 30);
	EXPECT_TRUE(m07[0][3] == 40);
	EXPECT_TRUE(m07[1][0] == 10);
	EXPECT_TRUE(m07[1][1] == 20);
	EXPECT_TRUE(m07[1][2] == 30);
	EXPECT_TRUE(m07[1][3] == 40);
	EXPECT_TRUE(m07[2][0] == 10);
	EXPECT_TRUE(m07[2][1] == 20);
	EXPECT_TRUE(m07[2][2] == 30);
	EXPECT_TRUE(m07[2][3] == 40);
	EXPECT_TRUE(m07[3][0] == 10);
	EXPECT_TRUE(m07[3][1] == 20);
	EXPECT_TRUE(m07[3][2] == 30);
	EXPECT_TRUE(m07[3][3] == 40);

	specter::mat4i m08(m00 * 10);
	EXPECT_TRUE(m08[0][0] == m00[0][0] * 10);
	EXPECT_TRUE(m08[0][1] == m00[0][1] * 10);
	EXPECT_TRUE(m08[0][2] == m00[0][2] * 10);
	EXPECT_TRUE(m08[0][3] == m00[0][3] * 10);
	EXPECT_TRUE(m08[1][0] == m00[1][0] * 10);
	EXPECT_TRUE(m08[1][1] == m00[1][1] * 10);
	EXPECT_TRUE(m08[1][2] == m00[1][2] * 10);
	EXPECT_TRUE(m08[1][3] == m00[1][3] * 10);
	EXPECT_TRUE(m08[2][0] == m00[2][0] * 10);
	EXPECT_TRUE(m08[2][1] == m00[2][1] * 10);
	EXPECT_TRUE(m08[2][2] == m00[2][2] * 10);
	EXPECT_TRUE(m08[2][3] == m00[2][3] * 10);
	EXPECT_TRUE(m08[3][0] == m00[3][0] * 10);
	EXPECT_TRUE(m08[3][1] == m00[3][1] * 10);
	EXPECT_TRUE(m08[3][2] == m00[3][2] * 10);
	EXPECT_TRUE(m08[3][3] == m00[3][3] * 10);

	specter::mat4i m09(m08 / 10);
	EXPECT_TRUE(m09[0][0] == m00[0][0]);
	EXPECT_TRUE(m09[0][1] == m00[0][1]);
	EXPECT_TRUE(m09[0][2] == m00[0][2]);
	EXPECT_TRUE(m09[0][3] == m00[0][3]);

	EXPECT_TRUE(m09[1][0] == m00[1][0]);
	EXPECT_TRUE(m09[1][1] == m00[1][1]);
	EXPECT_TRUE(m09[1][2] == m00[1][2]);
	EXPECT_TRUE(m09[1][3] == m00[1][3]);

	EXPECT_TRUE(m09[2][0] == m00[2][0]);
	EXPECT_TRUE(m09[2][1] == m00[2][1]);
	EXPECT_TRUE(m09[2][2] == m00[2][2]);
	EXPECT_TRUE(m09[2][3] == m00[2][3]);

	EXPECT_TRUE(m09[3][0] == m00[3][0]);
	EXPECT_TRUE(m09[3][1] == m00[3][1]);
	EXPECT_TRUE(m09[3][2] == m00[3][2]);
	EXPECT_TRUE(m09[3][3] == m00[3][3]);

	specter::mat4i m10(m00);
	m10 *= 10;
	EXPECT_TRUE(m10[0][0] == m00[0][0] * 10);
	EXPECT_TRUE(m10[0][1] == m00[0][1] * 10);
	EXPECT_TRUE(m10[0][2] == m00[0][2] * 10);
	EXPECT_TRUE(m10[0][3] == m00[0][3] * 10);
	EXPECT_TRUE(m10[1][0] == m00[1][0] * 10);
	EXPECT_TRUE(m10[1][1] == m00[1][1] * 10);
	EXPECT_TRUE(m10[1][2] == m00[1][2] * 10);
	EXPECT_TRUE(m10[1][3] == m00[1][3] * 10);
	EXPECT_TRUE(m10[2][0] == m00[2][0] * 10);
	EXPECT_TRUE(m10[2][1] == m00[2][1] * 10);
	EXPECT_TRUE(m10[2][2] == m00[2][2] * 10);
	EXPECT_TRUE(m10[2][3] == m00[2][3] * 10);
	EXPECT_TRUE(m10[3][0] == m00[3][0] * 10);
	EXPECT_TRUE(m10[3][1] == m00[3][1] * 10);
	EXPECT_TRUE(m10[3][2] == m00[3][2] * 10);
	EXPECT_TRUE(m10[3][3] == m00[3][3] * 10);

	specter::mat4i m11(m08);
	m11 /= 10;
	EXPECT_TRUE(m11[0][0] == m00[0][0]);
	EXPECT_TRUE(m11[0][1] == m00[0][1]);
	EXPECT_TRUE(m11[0][2] == m00[0][2]);
	EXPECT_TRUE(m11[0][3] == m00[0][3]);
	EXPECT_TRUE(m11[1][0] == m00[1][0]);
	EXPECT_TRUE(m11[1][1] == m00[1][1]);
	EXPECT_TRUE(m11[1][2] == m00[1][2]);
	EXPECT_TRUE(m11[1][3] == m00[1][3]);
	EXPECT_TRUE(m11[2][0] == m00[2][0]);
	EXPECT_TRUE(m11[2][1] == m00[2][1]);
	EXPECT_TRUE(m11[2][2] == m00[2][2]);
	EXPECT_TRUE(m11[2][3] == m00[2][3]);
	EXPECT_TRUE(m11[3][0] == m00[3][0]);
	EXPECT_TRUE(m11[3][1] == m00[3][1]);
	EXPECT_TRUE(m11[3][2] == m00[3][2]);
	EXPECT_TRUE(m11[3][3] == m00[3][3]);
}

TEST(access, mat4) {
	specter::vec4i col0(1, 1, 1, 1);
	specter::vec4i col1(2, 2, 2, 2);
	specter::vec4i col2(3, 3, 3, 3);
	specter::vec4i col3(4, 4, 4, 4);
	specter::mat4i m00(col0, col1, col2, col3);

	EXPECT_TRUE(m00[0][0] == 1);
	EXPECT_TRUE(m00[0][1] == 2);
	EXPECT_TRUE(m00[0][2] == 3);
	EXPECT_TRUE(m00[0][3] == 4);
	EXPECT_TRUE(m00[1][0] == 1);
	EXPECT_TRUE(m00[1][1] == 2);
	EXPECT_TRUE(m00[1][2] == 3);
	EXPECT_TRUE(m00[1][3] == 4);
	EXPECT_TRUE(m00[2][0] == 1);
	EXPECT_TRUE(m00[2][1] == 2);
	EXPECT_TRUE(m00[2][2] == 3);
	EXPECT_TRUE(m00[2][3] == 4);
	EXPECT_TRUE(m00[2][0] == 1);
	EXPECT_TRUE(m00[2][1] == 2);
	EXPECT_TRUE(m00[2][2] == 3);
	EXPECT_TRUE(m00[2][3] == 4);

	specter::mat4i m01(col0, col1, col2, col3);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			m01[i][j] = i * 4 + j;
		}
	}
	EXPECT_TRUE(m01[0][0] == 0);
	EXPECT_TRUE(m01[0][1] == 1);
	EXPECT_TRUE(m01[0][2] == 2);
	EXPECT_TRUE(m01[0][3] == 3);
	EXPECT_TRUE(m01[1][0] == 4);
	EXPECT_TRUE(m01[1][1] == 5);
	EXPECT_TRUE(m01[1][2] == 6);
	EXPECT_TRUE(m01[1][3] == 7);
	EXPECT_TRUE(m01[2][0] == 8);
	EXPECT_TRUE(m01[2][1] == 9);
	EXPECT_TRUE(m01[2][2] == 10);
	EXPECT_TRUE(m01[2][3] == 11);
	EXPECT_TRUE(m01[3][0] == 12);
	EXPECT_TRUE(m01[3][1] == 13);
	EXPECT_TRUE(m01[3][2] == 14);
	EXPECT_TRUE(m01[3][3] == 15);

	specter::vec4i gcol0 = m00.col(0);
	specter::vec4i gcol1 = m00.col(1);
	specter::vec4i gcol2 = m00.col(2);
	specter::vec4i gcol3 = m00.col(3);
	EXPECT_TRUE(gcol0[0] == 1);
	EXPECT_TRUE(gcol0[1] == 1);
	EXPECT_TRUE(gcol0[2] == 1);
	EXPECT_TRUE(gcol0[3] == 1);
	EXPECT_TRUE(gcol1[0] == 2);
	EXPECT_TRUE(gcol1[1] == 2);
	EXPECT_TRUE(gcol1[2] == 2);
	EXPECT_TRUE(gcol1[3] == 2);
	EXPECT_TRUE(gcol2[0] == 3);
	EXPECT_TRUE(gcol2[1] == 3);
	EXPECT_TRUE(gcol2[2] == 3);
	EXPECT_TRUE(gcol2[3] == 3);
	EXPECT_TRUE(gcol3[0] == 4);
	EXPECT_TRUE(gcol3[1] == 4);
	EXPECT_TRUE(gcol3[2] == 4);
	EXPECT_TRUE(gcol3[3] == 4);

	specter::vec4i grow0 = m00.row(0);
	specter::vec4i grow1 = m00.row(1);
	specter::vec4i grow2 = m00.row(2);
	specter::vec4i grow3 = m00.row(3);
	EXPECT_TRUE(grow0[0] == 1);
	EXPECT_TRUE(grow0[1] == 2);
	EXPECT_TRUE(grow0[2] == 3);
	EXPECT_TRUE(grow0[3] == 4);
	EXPECT_TRUE(grow1[0] == 1);
	EXPECT_TRUE(grow1[1] == 2);
	EXPECT_TRUE(grow1[2] == 3);
	EXPECT_TRUE(grow1[3] == 4);
	EXPECT_TRUE(grow2[0] == 1);
	EXPECT_TRUE(grow2[1] == 2);
	EXPECT_TRUE(grow2[2] == 3);
	EXPECT_TRUE(grow2[3] == 4);
	EXPECT_TRUE(grow3[0] == 1);
	EXPECT_TRUE(grow3[1] == 2);
	EXPECT_TRUE(grow3[2] == 3);
	EXPECT_TRUE(grow3[3] == 4);
}

TEST(miscellaneous, mat4) {
	specter::mat4i id(1);
	EXPECT_TRUE(id[0][0] == 1);
	EXPECT_TRUE(id[1][1] == 1);
	EXPECT_TRUE(id[2][2] == 1);
	EXPECT_TRUE(id[3][3] == 1);
	EXPECT_TRUE(id[0][1] == 0);
	EXPECT_TRUE(id[0][2] == 0);
	EXPECT_TRUE(id[0][3] == 0);
	EXPECT_TRUE(id[1][0] == 0);
	EXPECT_TRUE(id[1][2] == 0);
	EXPECT_TRUE(id[1][3] == 0);
	EXPECT_TRUE(id[2][0] == 0);
	EXPECT_TRUE(id[2][1] == 0);
	EXPECT_TRUE(id[2][3] == 0);
	EXPECT_TRUE(id[3][0] == 0);
	EXPECT_TRUE(id[3][1] == 0);
	EXPECT_TRUE(id[3][2] == 0);
	
	EXPECT_TRUE(specter::isIdentity(id));
	EXPECT_TRUE(specter::isIdentityCorrected(id));

	specter::vec4i col0(1, 1, 1, 1);
	specter::vec4i col1(2, 2, 2, 2);
	specter::vec4i col2(3, 3, 3, 3);
	specter::vec4i col3(4, 4, 4, 4);
	specter::mat4i m00(col0, col1, col2, col3);

	EXPECT_TRUE(specter::trace(m00) == 10);

	specter::mat4i m01 = specter::transpose(m00);
	EXPECT_TRUE(m00[0][0] == m01[0][0]);
	EXPECT_TRUE(m00[0][1] == m01[1][0]);
	EXPECT_TRUE(m00[0][2] == m01[2][0]);
	EXPECT_TRUE(m00[0][3] == m01[3][0]);
	EXPECT_TRUE(m00[1][0] == m01[0][1]);
	EXPECT_TRUE(m00[1][1] == m01[1][1]);
	EXPECT_TRUE(m00[1][2] == m01[2][1]);
	EXPECT_TRUE(m00[1][3] == m01[3][1]);
	EXPECT_TRUE(m00[2][0] == m01[0][2]);
	EXPECT_TRUE(m00[2][1] == m01[1][2]);
	EXPECT_TRUE(m00[2][2] == m01[2][2]);
	EXPECT_TRUE(m00[2][3] == m01[3][2]);
	EXPECT_TRUE(m00[3][0] == m01[0][3]);
	EXPECT_TRUE(m00[3][1] == m01[1][3]);
	EXPECT_TRUE(m00[3][2] == m01[2][3]);
	EXPECT_TRUE(m00[3][3] == m01[3][3]);
}