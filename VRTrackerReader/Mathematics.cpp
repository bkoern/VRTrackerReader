#include "Mathematics.h"

Quaternion toQuaternion(const Matrix4X4& M)
{
	Quaternion quat;
	float	s;

	// Check diagonal (trace)
	const float tr = M.M[0][0] + M.M[1][1] + M.M[2][2];

	if (tr > 0.0f)
	{
		float InvS = 1.f / std::sqrt(tr + 1.f);
		quat.w = -0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		quat.y = (M.M[1][2] - M.M[2][1]) * s;
		quat.z = (M.M[2][0] - M.M[0][2]) * s;
		quat.x = -(M.M[0][1] - M.M[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int32_t i = 0;

		if (M.M[1][1] > M.M[0][0])
			i = 1;

		if (M.M[2][2] > M.M[i][i])
			i = 2;

		static const int32_t nxt[3] = { 1, 2, 0 };
		const int32_t j = nxt[i];
		const int32_t k = nxt[j];

		s = M.M[i][i] - M.M[j][j] - M.M[k][k] + 1.0f;

		float InvS = 1.f / std::sqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (M.M[j][k] - M.M[k][j]) * s;
		qt[j] = (M.M[i][j] + M.M[j][i]) * s;
		qt[k] = (M.M[i][k] + M.M[k][i]) * s;

		quat.y = qt[0];
		quat.z = qt[1];
		quat.x = -qt[2];
		quat.w = -qt[3];
	}
	return quat;
}

std::vector<float> getPosition(vr::HmdMatrix34_t& matrix) {
	std::vector<float> position;

	position.push_back(-matrix.m[2][3] * METERTOUNREALUNITS);
	position.push_back(matrix.m[0][3] * METERTOUNREALUNITS);
	position.push_back(matrix.m[1][3] * METERTOUNREALUNITS);

	return position;
}

RPY calculateYPR(const Quaternion& quaternion) {
	RPY rotation;

	double sqw = quaternion.w * quaternion.w;
	double sqx = quaternion.x * quaternion.x;
	double sqy = quaternion.y * quaternion.y;
	double sqz = quaternion.z * quaternion.z;

	double roll = std::atan2(2.0 * (quaternion.y * quaternion.z + quaternion.w * quaternion.x), sqw - sqx - sqy + sqz);
	double pitch = std::asin(-2.0 * (quaternion.x * quaternion.z - quaternion.w * quaternion.y));
	double yaw = std::atan2(2.0 * (quaternion.x * quaternion.y + quaternion.w * quaternion.z), sqw + sqx - sqy - sqz);

	rotation.roll = (float)-(roll * (180.0 / M_PI));
	rotation.pitch = (float)-(pitch * (180.0 / M_PI));
	rotation.yaw = (float)(yaw * (180.0 / M_PI));

	return rotation;
}
