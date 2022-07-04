//#pragma once
//#include "Actor.h"
//#include "AssistMath.h"
//#include "GlobalStateObject.h"
//#include <vector>
//#include <optional>
//#include <iostream>
//
///*
//		inline float doBasicCollisionH5(Actor& actor, DirectX::XMFLOAT3& tri1Norm,
//			DirectX::XMFLOAT3X3& tri) {
//
//			float maxDist{ 0.0f };
//			DirectX::XMMATRIX transformMx = actor.getModel().getObjects().at(0).getTransformMX();
//
//			// Ax + By + Cz = D
//			float A = tri1Norm.x;
//			float B = tri1Norm.y;
//			float C = tri1Norm.z;
//			float a = tri._11;
//			float b = tri._12;
//			float c = tri._13;
//			float D = -(A * a + B * b + C * c);
//
//			for (auto& o : actor.getModel().getITLData().m_objects) {
//				for (auto& v : o.pos) {
//
//					DirectX::XMVECTOR point{ v.x, v.y, v.z, 1.0f };
//					point = DirectX::XMVector4Transform(point, transformMx);
//					float x0 = point.m128_f32[0];
//					float y0 = point.m128_f32[1];
//					float z0 = point.m128_f32[2];
//
//					float distance = (A * x0 + B * y0 + C * z0 + D)
//						/ std::sqrt(A * A + B * B + C * C);
//
//					if (distance < maxDist) maxDist = distance;
//
//				}
//			}
//			return maxDist;
//		}
//
//		inline DirectX::XMFLOAT3 doBasicCollisionH4(DirectX::XMFLOAT3X3& tri) {
//
//			DirectX::XMFLOAT3 point0{ tri._11, tri._12, tri._13 };
//			DirectX::XMFLOAT3 point1{ tri._21, tri._22, tri._23 };
//			DirectX::XMFLOAT3 point2{ tri._31, tri._32, tri._33 };
//
//			DirectX::XMFLOAT3 V{ point1.x - point0.x, point1.y - point0.y, point1.z - point0.z };
//			DirectX::XMFLOAT3 W{ point2.x - point0.x, point2.y - point0.y, point2.z - point0.z };
//
//			float normX = (V.y * W.z) - (V.z * W.y);
//			float normY = (V.z * W.x) - (V.x * W.z);
//			float normZ = (V.x * W.y) - (V.y * W.x);
//
//			float normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);
//
//			float Ax = normX / normConst;
//			float Ay = normY / normConst;
//			float Az = normZ / normConst;
//
//			float test = std::sqrtf(Ax * Ax + Ay * Ay + Az * Az);
//
//			return { Ax, Ay, Az };
//		}
//
//		inline void doBasicCollisionH3(Actor& actor, std::pair<DirectX::XMFLOAT3X3, DirectX::XMFLOAT3X3>& collidedtris) {
//
//			DirectX::XMFLOAT3X3 tri0{ collidedtris.first };
//			DirectX::XMFLOAT3X3 tri1{ collidedtris.second };
//
//			DirectX::XMFLOAT3 tri1Norm = doBasicCollisionH4(tri1);
//
//			float distance = -doBasicCollisionH5(actor, tri1Norm, tri1);
//
//			for (auto& o : actor.getModel().getObjects()) {
//
//				//DirectX::XMFLOAT3 momentumVector{ o.getSpeed().dx, o.getSpeed().dy, o.getSpeed().dz };
//
//				o.getPos().x += distance * tri1Norm.x;
//				o.getPos().y += distance * tri1Norm.y;
//				o.getPos().z += distance * tri1Norm.z;
//			}
//
//			for (auto& o : actor.getModel().getObjects()) {
//
//				o.getSpeed().dx = -o.getSpeed().dx / 5.0f;
//				o.getSpeed().dy = -o.getSpeed().dy / 5.0f;
//				o.getSpeed().dz = -o.getSpeed().dz / 5.0f;
//			}
//
//			//for (auto& o : actor.getModel().getObjects()) {
//			//	if (o.getSpeed().dx > 0.5f)
//			//		o.getSpeed().dx = -o.getSpeed().dx / 5.0f;
//			//	else o.getSpeed().dx = 0.0f;
//
//			//	if (o.getSpeed().dy > 0.5f)
//			//		o.getSpeed().dy = -o.getSpeed().dy / 5.0f;
//			//	else o.getSpeed().dy = 0.0f;
//
//			//	if (o.getSpeed().dz > 0.5f)
//			//		o.getSpeed().dz = -o.getSpeed().dz / 5.0f;
//			//	else o.getSpeed().dz = 0.0f;
//			//}
//		}
//
//		inline std::optional<std::pair<DirectX::XMFLOAT3X3, DirectX::XMFLOAT3X3>>
//			doBasicCollisionH2(std::vector<DirectX::XMFLOAT3>& actor0vertices, std::vector<DirectX::XMFLOAT3X3>& actor1tris) {
//
//
//
//			for (auto& v : actor0vertices) {
//
//				bool IsIntersected{ true };
//
//				for (auto& tri1 : actor1tris) {
//
//					DirectX::XMFLOAT3 tri1Norm = doBasicCollisionH4(tri1);
//
//					float A = tri1Norm.x;
//					float B = tri1Norm.y;
//					float C = tri1Norm.z;
//					float a = tri1._11;
//					float b = tri1._12;
//					float c = tri1._13;
//					float D = -(A * a + B * b + C * c);
//
//					float distance = (A * v.x + B * v.y + C * v.z + D)
//						/ std::sqrt(A * A + B * B + C * C);
//
//					if (distance > 0.0f) {
//						IsIntersected = false;
//						break;
//					}
//
//					//if (DirectX::TriangleTests::Intersects(tri0vec0, tri0vec1, tri0vec2, tri1vec0, tri1vec1, tri1vec2)) {
//					//	std::pair<DirectX::XMFLOAT3, DirectX::XMFLOAT3X3> collidedtri{ v, tri1 };
//					//	return collidedtri;
//					//}
//				}
//
//				if (IsIntersected)
//
//			}
//			return std::nullopt;
//		}
//
//		inline void doBasicCollisionH1(Actor& actor0, Actor& actor1,
//			std::vector<DirectX::XMFLOAT3>& actor0vertices,
//			std::vector<DirectX::XMFLOAT3X3>& actor1tris) {
//				{
//					UINT iter = 0u;
//					for (auto& v : actor0.getModel().getITLData().m_pos) {
//						actor0vertices.at(iter) = { v.x, v.y, v.z };
//						iter++;
//					}
//				}
//				{
//					UINT triIter = 0u;
//					UINT posIter = 0u;
//
//					DirectX::XMMATRIX transformMx = actor1.getModel().getObjects().at(0).getTransformMX();
//
//					while (triIter < actor1tris.size()) {
//						float x0 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter).x;
//						float y0 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter).y;
//						float z0 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter).z;
//						DirectX::XMVECTOR point0 = DirectX::XMVectorSet(x0, y0, z0, 1.0f);
//						point0 = DirectX::XMVector4Transform(point0, transformMx);
//						float x1 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter + 1).x;
//						float y1 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter + 1).y;
//						float z1 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter + 1).z;
//						DirectX::XMVECTOR point1 = DirectX::XMVectorSet(x1, y1, z1, 1.0f);
//						point1 = DirectX::XMVector4Transform(point1, transformMx);
//						float x2 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter + 2).x;
//						float y2 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter + 2).y;
//						float z2 = actor1.getModel().getITLData().m_objects.at(0).pos.at(posIter + 2).z;
//						DirectX::XMVECTOR point2 = DirectX::XMVectorSet(x2, y2, z2, 1.0f);
//						point2 = DirectX::XMVector4Transform(point2, transformMx);
//						DirectX::XMFLOAT3 point0f = { point0.m128_f32[0], point0.m128_f32[1], point0.m128_f32[2] };
//						DirectX::XMFLOAT3 point1f = { point1.m128_f32[0], point1.m128_f32[1], point1.m128_f32[2] };
//						DirectX::XMFLOAT3 point2f = { point2.m128_f32[0], point2.m128_f32[1], point2.m128_f32[2] };
//						actor1tris[triIter]._11 = point0f.x;
//						actor1tris[triIter]._12 = point0f.y;
//						actor1tris[triIter]._13 = point0f.z;
//						actor1tris[triIter]._21 = point1f.x;
//						actor1tris[triIter]._22 = point1f.y;
//						actor1tris[triIter]._23 = point1f.z;
//						actor1tris[triIter]._31 = point2f.x;
//						actor1tris[triIter]._32 = point2f.y;
//						actor1tris[triIter]._33 = point2f.z;
//						triIter++;
//						posIter += 3;
//					}
//				}
//		}
//
//		inline void doBasicCollisionH0(Actor& actor0, Actor& actor1) {
//			std::vector<DirectX::XMFLOAT3> actor0points{};
//			std::vector<DirectX::XMFLOAT3X3> actor1tris{};
//			actor0points.resize(actor0.getModel().getITLData().m_pos.size());
//			actor1tris.resize(actor1.getModel().getITLData().m_objects.at(0).pos.size() / 3);
//			doBasicCollisionH1(actor0, actor1, actor0points, actor1tris);
//			std::optional<std::pair<DirectX::XMFLOAT3X3, DirectX::XMFLOAT3X3>> collisionResult = doBasicCollisionH2(actor0points, actor1tris);
//			if (collisionResult != std::nullopt) {
//				doBasicCollisionH3(actor0, collisionResult.value());
//			}
//		}
//		*/
//
//namespace GID {
//	namespace Scripts {
//
//		namespace Physics {
//
//			using namespace AssistMath;
//
//			struct Triangle {
//				FAMVECTOR p1{};
//				FAMVECTOR p2{};
//				FAMVECTOR p3{};
//			};
//			struct Line {
//				FAMVECTOR p1{};
//				FAMVECTOR p2{};
//			};
//
//			void doBasicCollision(Actor& actor, std::vector<Actor>& actors) {
//
//				// Gather all tris from the first actor
//				std::vector<Triangle> actor0Tris{};
//				{
//					std::vector<Triangle> tris{};
//					AssistMath::FAMMATRIX transformMx = actor.getModel().getObjects().at(0).getTransformMx();
//					UINT iter = 0u;
//					AssistMath::FAMVECTOR v0{};
//					AssistMath::FAMVECTOR v1{};
//					AssistMath::FAMVECTOR v2{};
//					for (auto& v : actor.getModel().getITLData().m_objects.at(0).pos) {
//
//						AssistMath::FAMVECTOR vector{ AssistMath::AMLoadFloat4({ v.x, v.y, v.z, 1.0f }) };
//						vector = FAMVector4Transform(vector, transformMx);
//
//						if (iter == 0) v0 = { AssistMath::AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
//						if (iter == 1) v1 = { AssistMath::AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
//						if (iter == 2) {
//							v2 = { vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] };
//							iter = 0;
//							actor0Tris.push_back({ v0, v1, v2 });
//							continue;
//						}
//						iter++;
//					}
//				}
//
//				// Cycle through all of the actors to check for collisions
//				for (auto& a : actors) {
//
//					// Don't want to check collisions with the same actor
//					if (a.mActorID != actor.mActorID) {
//
//						// Gather all tris from the second actor
//						std::vector<Triangle> actor1Tris{};
//						{
//							std::vector<Triangle> tris{};
//							AssistMath::FAMMATRIX transformMx = a.getModel().getObjects().at(0).getTransformMx();
//							UINT iter = 0u;
//							AssistMath::FAMVECTOR v0{};
//							AssistMath::FAMVECTOR v1{};
//							AssistMath::FAMVECTOR v2{};
//							for (auto& v : a.getModel().getITLData().m_objects.at(0).pos) {
//
//								AssistMath::FAMVECTOR vector{ AssistMath::AMLoadFloat4({ v.x, v.y, v.z, 1.0f }) };
//								vector = FAMVector4Transform(vector, transformMx);
//
//								if (iter == 0) v0 = { AssistMath::AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
//								if (iter == 1) v1 = { AssistMath::AMLoadFloat3({ vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] }) };
//								if (iter == 2) {
//									v2 = { vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2] };
//									iter = 0;
//									actor1Tris.push_back({ v0, v1, v2 });
//									continue;
//								}
//								iter++;
//							}
//						}
//
//						// Determine if a triangle from actor 1 collides with a triangle of actor 2 (O(n) = n^2, make faster)
//						float result{};
//						FAMVECTOR triNorm{};
//						Triangle collidedTri{};
//						{
//							float intersectingDist{};
//
//							for (auto& tri0 : actor0Tris) {
//								for (auto& tri1 : actor1Tris) {
//
//									std::array<Line, 3u> lines = {
//										{
//											{ tri0.p1, tri0.p2 },
//											{ tri0.p1, tri0.p3 },
//											{ tri0.p2, tri0.p3 }
//										}
//									};
//
//									for (UINT i = 0; i < lines.size(); i++) {
//
//										enum Point {
//											x,
//											y,
//											z
//										};
//
//										AMFLOAT3 V{
//											tri1.p2.m128_f32[x] - tri1.p1.m128_f32[x],
//											tri1.p2.m128_f32[y] - tri1.p1.m128_f32[y],
//											tri1.p2.m128_f32[z] - tri1.p1.m128_f32[z]
//										};
//
//										AMFLOAT3 W{
//											tri1.p3.m128_f32[x] - tri1.p1.m128_f32[x],
//											tri1.p3.m128_f32[y] - tri1.p1.m128_f32[y],
//											tri1.p3.m128_f32[z] - tri1.p1.m128_f32[z]
//										};
//
//										float normX = (V.y * W.z) - (V.z * W.y);
//										float normY = (V.z * W.x) - (V.x * W.z);
//										float normZ = (V.x * W.y) - (V.y * W.x);
//
//										float normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);
//
//										float Ax = normX / normConst;
//										float Ay = normY / normConst;
//										float Az = normZ / normConst;
//
//										float test = std::sqrtf(Ax * Ax + Ay * Ay + Az * Az);
//
//										float A = Ax;
//										float B = Ay;
//										float C = Az;
//										float a = tri1.p1.m128_f32[x];
//										float b = tri1.p1.m128_f32[y];
//										float c = tri1.p1.m128_f32[z];
//										float D = -(A * a + B * b + C * c);
//
//										float triA = A;
//										float triB = B;
//										float triC = C;
//
//										float result0 = (A * lines[i].p1.m128_f32[x] + B * lines[i].p1.m128_f32[y] + C * lines[i].p1.m128_f32[z] + D)
//											/ std::sqrtf(A * A + B * B + C * C);
//
//										float result1 = (A * lines[i].p2.m128_f32[x] + B * lines[i].p2.m128_f32[y] + C * lines[i].p2.m128_f32[z] + D)
//											/ std::sqrtf(A * A + B * B + C * C);
//
//										bool isDifferentSign{ (signbit(result0) == signbit(result1)) ? false : true };
//
//										if (isDifferentSign) {
//
//											AssistMath::FAMVECTOR outsidePoint{ (result1 > 0.0f) ? lines[i].p2 : lines[i].p1 };
//											AssistMath::FAMVECTOR insidePoint{ (result1 > 0.0f) ? lines[i].p1 : lines[i].p2 };
//
//											Triangle plane0tri{};
//											plane0tri.p1 = outsidePoint;
//											plane0tri.p2 = tri1.p2;
//											plane0tri.p3 = tri1.p1;
//											Triangle plane1tri{};
//											plane1tri.p1 = outsidePoint;
//											plane1tri.p2 = tri1.p3;
//											plane1tri.p3 = tri1.p2;
//											Triangle plane2tri{};
//											plane2tri.p1 = outsidePoint;
//											plane2tri.p2 = tri1.p1;
//											plane2tri.p3 = tri1.p3;
//
//											AMFLOAT3 vector0{
//												plane0tri.p2.m128_f32[x] - plane0tri.p1.m128_f32[x],
//												plane0tri.p2.m128_f32[y] - plane0tri.p1.m128_f32[y],
//												plane0tri.p2.m128_f32[z] - plane0tri.p1.m128_f32[z] };
//											AMFLOAT3 vector1{
//												plane0tri.p3.m128_f32[x] - plane0tri.p1.m128_f32[x],
//												plane0tri.p3.m128_f32[y] - plane0tri.p1.m128_f32[y],
//												plane0tri.p3.m128_f32[z] - plane0tri.p1.m128_f32[z] };
//
//											float normX = (vector0.y * vector1.z) - (vector0.z * vector1.y);
//											float normY = (vector0.z * vector1.x) - (vector0.x * vector1.z);
//											float normZ = (vector0.x * vector1.y) - (vector0.y * vector1.x);
//
//											float normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);
//
//											float Ax = normX / normConst;
//											float Ay = normY / normConst;
//											float Az = normZ / normConst;
//
//											float A = Ax;
//											float B = Ay;
//											float C = Az;
//											float a = outsidePoint.m128_f32[x];
//											float b = outsidePoint.m128_f32[y];
//											float c = outsidePoint.m128_f32[z];
//											float D = -(A * a + B * b + C * c);
//
//											float result = (A * insidePoint.m128_f32[x] + B * insidePoint.m128_f32[y] + C * insidePoint.m128_f32[z] + D)
//												/ std::sqrt(A * A + B * B + C * C);
//
//											if (result < 0.0f) continue;
//
//											vector0 = {
//												plane1tri.p2.m128_f32[x] - plane1tri.p1.m128_f32[x],
//												plane1tri.p2.m128_f32[y] - plane1tri.p1.m128_f32[y],
//												plane1tri.p2.m128_f32[z] - plane1tri.p1.m128_f32[z]
//											};
//
//											vector1 = {
//												plane1tri.p3.m128_f32[x] - plane1tri.p1.m128_f32[x],
//												plane1tri.p3.m128_f32[y] - plane1tri.p1.m128_f32[y],
//												plane1tri.p3.m128_f32[z] - plane1tri.p1.m128_f32[z]
//											};
//
//											normX = (vector0.y * vector1.z) - (vector0.z * vector1.y);
//											normY = (vector0.z * vector1.x) - (vector0.x * vector1.z);
//											normZ = (vector0.x * vector1.y) - (vector0.y * vector1.x);
//
//											normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);
//
//											Ax = normX / normConst;
//											Ay = normY / normConst;
//											Az = normZ / normConst;
//
//											A = Ax;
//											B = Ay;
//											C = Az;
//											a = outsidePoint.m128_f32[x];
//											b = outsidePoint.m128_f32[y];
//											c = outsidePoint.m128_f32[z];
//											D = -(A * a + B * b + C * c);
//
//											result = (A * insidePoint.m128_f32[x] + B * insidePoint.m128_f32[y] + C * insidePoint.m128_f32[z] + D)
//												/ std::sqrtf(A * A + B * B + C * C);
//
//											if (result < 0.0f) continue;
//
//											vector0 = {
//												plane2tri.p2.m128_f32[x] - plane2tri.p1.m128_f32[x],
//												plane2tri.p2.m128_f32[y] - plane2tri.p1.m128_f32[y],
//												plane2tri.p2.m128_f32[z] - plane2tri.p1.m128_f32[z]
//											};
//
//											vector1 = {
//												plane2tri.p3.m128_f32[x] - plane2tri.p1.m128_f32[x],
//												plane2tri.p3.m128_f32[y] - plane2tri.p1.m128_f32[y],
//												plane2tri.p3.m128_f32[z] - plane2tri.p1.m128_f32[z]
//											};
//
//											normX = (vector0.y * vector1.z) - (vector0.z * vector1.y);
//											normY = (vector0.z * vector1.x) - (vector0.x * vector1.z);
//											normZ = (vector0.x * vector1.y) - (vector0.y * vector1.x);
//
//											normConst = std::sqrtf(normX * normX + normY * normY + normZ * normZ);
//
//											Ax = normX / normConst;
//											Ay = normY / normConst;
//											Az = normZ / normConst;
//
//											A = Ax;
//											B = Ay;
//											C = Az;
//											a = outsidePoint.m128_f32[x];
//											b = outsidePoint.m128_f32[y];
//											c = outsidePoint.m128_f32[z];
//											D = -(A * a + B * b + C * c);
//
//											result = (A * insidePoint.m128_f32[x] + B * insidePoint.m128_f32[y] + C * insidePoint.m128_f32[z] + D)
//												/ std::sqrtf(A * A + B * B + C * C);
//
//											if (result >= 0.0f) {
//												if (result0 > 0.0f)
//													if (result0 > intersectingDist)
//														intersectingDist = result1;
//													else intersectingDist = result0;
//												collidedTri = tri1;
//												triNorm = { AMLoadFloat4({ triA, triB, triC, 0.0f }) };
//											}
//										}
//									}
//								}
//							}
//							result = intersectingDist;
//						}
//
//						// If the result is negative, then we know that a collision has taken place
//						// (result is the amount of distance intersected)
//						if (result < 0.0f) {
//
//							FAMVECTOR velocity = actor.getModel().getObjects().at(0).getSpeed().deltaTranslation;
//							FAMVECTOR translation = actor.getModel().getObjects().at(0).getPos().translation;
//
//							enum Point {
//								x,
//								y,
//								z
//							};
//
//							// Find the length intersected along the velocity vector of the object
//							float fVelocity = FAMVector4Magnitude(FAMVector4Negate(velocity));
//
//							FAMVECTOR _0 = FAMVector4DotProduct(triNorm, FAMVector4Negate(velocity));
//							float _1 = FAMVector4SumVector(_0);
//							float _2 = FAMVector4Magnitude(triNorm) * fVelocity;
//							float _3 = _1 / _2;
//
//							float angle = std::acosf(_3);
//
//							#if defined(_DEBUG)
//							std::cout << "Collision detected!!! Reactionary movement: " << result / std::cos(angle) << '\n';
//							#endif
//
//							std::cout << AMConvertToDegrees(angle) << '\n';
//
//							// Push the object out 
//							actor.getModel().getObjects().at(0).getPos().translation
//								= _mm_add_ps(translation,
//									_mm_mul_ps(FAMVector4Negate(velocity),
//										_mm_set_ps1(result / std::cos(angle))));
//							// ---
//
//
//
//							// Determine the reflected momentum vector of the actor
//							FAMVECTOR refl = _mm_sub_ps(velocity, _mm_mul_ps(_mm_set_ps1(2.0f *
//								FAMVector4SumVector(FAMVector4DotProduct(velocity, triNorm))), triNorm));
//							// ---
//
//
//
//							// Set the actor's momentum vector to the reflected momentum vector
//							actor.getModel().getObjects().at(0).getSpeed().deltaTranslation = refl;
//
//							float bk = 0.0f;
//
//							/*actor0.getModel().getObjects().at(0).getSpeed().deltaTranslation.m128_f32[1]
//								= -actor0.getModel().getObjects().at(0).getSpeed().deltaTranslation.m128_f32[1];*/
//						}
//					}
//				}
//			}
//		}
//	}
//}