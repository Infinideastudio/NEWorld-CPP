#include "frustum.h"

namespace Frustum{
	double m_Frustum[24];
	double proj[16], modl[16];
	double clip[16];

	void normalize(int side){
		double f0 = m_Frustum[side + 0];
		double f1 = m_Frustum[side + 1];
		double f2 = m_Frustum[side + 2];
		double magnitude = sqrt(f0 * f0 + f1 * f1 + f2 * f2);

		m_Frustum[side + 0] /= magnitude;
		m_Frustum[side + 1] /= magnitude;
		m_Frustum[side + 2] /= magnitude;
		m_Frustum[side + 3] /= magnitude;
	}

	void calc(){
		glGetDoublev(GL_PROJECTION_MATRIX, proj);
		glGetDoublev(GL_MODELVIEW_MATRIX, modl);

		clip[0] = modl[0] * proj[0] + modl[1] * proj[4] + modl[2] * proj[8] + modl[3] * proj[12];
		clip[1] = modl[0] * proj[1] + modl[1] * proj[5] + modl[2] * proj[9] + modl[3] * proj[13];
		clip[2] = modl[0] * proj[2] + modl[1] * proj[6] + modl[2] * proj[10] + modl[3] * proj[14];
		clip[3] = modl[0] * proj[3] + modl[1] * proj[7] + modl[2] * proj[11] + modl[3] * proj[15];

		clip[4] = modl[4] * proj[0] + modl[5] * proj[4] + modl[6] * proj[8] + modl[7] * proj[12];
		clip[5] = modl[4] * proj[1] + modl[5] * proj[5] + modl[6] * proj[9] + modl[7] * proj[13];
		clip[6] = modl[4] * proj[2] + modl[5] * proj[6] + modl[6] * proj[10] + modl[7] * proj[14];
		clip[7] = modl[4] * proj[3] + modl[5] * proj[7] + modl[6] * proj[11] + modl[7] * proj[15];

		clip[8] = modl[8] * proj[0] + modl[9] * proj[4] + modl[10] * proj[8] + modl[11] * proj[12];
		clip[9] = modl[8] * proj[1] + modl[9] * proj[5] + modl[10] * proj[9] + modl[11] * proj[13];
		clip[10] = modl[8] * proj[2] + modl[9] * proj[6] + modl[10] * proj[10] + modl[11] * proj[14];
		clip[11] = modl[8] * proj[3] + modl[9] * proj[7] + modl[10] * proj[11] + modl[11] * proj[15];

		clip[12] = modl[12] * proj[0] + modl[13] * proj[4] + modl[14] * proj[8] + modl[15] * proj[12];
		clip[13] = modl[12] * proj[1] + modl[13] * proj[5] + modl[14] * proj[9] + modl[15] * proj[13];
		clip[14] = modl[12] * proj[2] + modl[13] * proj[6] + modl[14] * proj[10] + modl[15] * proj[14];
		clip[15] = modl[12] * proj[3] + modl[13] * proj[7] + modl[14] * proj[11] + modl[15] * proj[15];

		m_Frustum[0] = clip[3] - clip[0];
		m_Frustum[1] = clip[7] - clip[4];
		m_Frustum[2] = clip[11] - clip[8];
		m_Frustum[3] = clip[15] - clip[12];

		normalize(0);

		m_Frustum[4] = clip[3] + clip[0];
		m_Frustum[5] = clip[7] + clip[4];
		m_Frustum[6] = clip[11] + clip[8];
		m_Frustum[7] = clip[15] + clip[12];

		normalize(4);

		m_Frustum[8] = clip[3] + clip[1];
		m_Frustum[9] = clip[7] + clip[5];
		m_Frustum[10] = clip[11] + clip[9];
		m_Frustum[11] = clip[15] + clip[13];

		normalize(8);

		m_Frustum[12] = clip[3] - clip[1];
		m_Frustum[13] = clip[7] - clip[5];
		m_Frustum[14] = clip[11] - clip[9];
		m_Frustum[15] = clip[15] - clip[13];

		normalize(12);

		m_Frustum[16] = clip[3] - clip[2];
		m_Frustum[17] = clip[7] - clip[6];
		m_Frustum[18] = clip[11] - clip[10];
		m_Frustum[19] = clip[15] - clip[14];

		normalize(16);

		m_Frustum[20] = clip[3] + clip[2];
		m_Frustum[21] = clip[7] + clip[6];
		m_Frustum[22] = clip[11] + clip[10];
		m_Frustum[23] = clip[15] + clip[14];

		normalize(20);

	}

	bool cubeInFrustum(double x1, double y1, double z1, double x2, double y2, double z2){
		int i = 0;
		while (i < 24){
			double f0 = m_Frustum[i + 0];
			double f1 = m_Frustum[i + 1];
			double f2 = m_Frustum[i + 2];
			double f3 = m_Frustum[i + 3];
			if ((f0 * x1 + f1 * y1 + f2 * z1 + f3 <= 0.0) &&
				(f0 * x2 + f1 * y1 + f2 * z1 + f3 <= 0.0) &&
				(f0 * x1 + f1 * y2 + f2 * z1 + f3 <= 0.0) &&
				(f0 * x2 + f1 * y2 + f2 * z1 + f3 <= 0.0) &&
				(f0 * x1 + f1 * y1 + f2 * z2 + f3 <= 0.0) &&
				(f0 * x2 + f1 * y1 + f2 * z2 + f3 <= 0.0) &&
				(f0 * x1 + f1 * y2 + f2 * z2 + f3 <= 0.0) &&
				(f0 * x2 + f1 * y2 + f2 * z2 + f3 <= 0.0))
				return false;
			i += 4;
		}

		return true;
	}

	bool aabbInFrustum(Hitbox::AABB aabb){
		return cubeInFrustum(aabb.xmin, aabb.ymin, aabb.zmin, aabb.xmax, aabb.ymax, aabb.zmax);
	}

}
