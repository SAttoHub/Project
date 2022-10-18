#include "GameCollisionManager.h"

bool GCM::CheckCollision(GameBaseCollider *col, std::string Tag) {
	// 総当りチェック
	itB = colliders.begin();
	for (; itB != colliders.end(); ++itB) {
		colB = *itB;
		// 球と球
		if (col->GetShapeType() == COL_SHAPE_CIRCLE &&
			colB->GetShapeType() == COL_SHAPE_CIRCLE &&
			colB->GetTag() == Tag) {
			Circle *SphereA = dynamic_cast<Circle *>(col);
			Circle *SphereB = dynamic_cast<Circle *>(colB);
			if (Collision2::CheckCircleCircle(*SphereA, *SphereB)) {
				return true;
			}
		}
		else if (col->GetShapeType() == COL_SHAPE_BOX &&
			colB->GetShapeType() == COL_SHAPE_CIRCLE &&
			colB->GetTag() == Tag) {
			Box *box = dynamic_cast<Box *>(col);
			Circle *sphere = dynamic_cast<Circle *>(colB);
			if (Collision2::CheckCircleBox(*sphere, *box)) {
				return true;
			}
		}
		else if (col->GetShapeType() == COL_SHAPE_CIRCLE &&
			colB->GetShapeType() == COL_SHAPE_BOX &&
			colB->GetTag() == Tag) {
			Box *box = dynamic_cast<Box *>(colB);
			Circle *sphere = dynamic_cast<Circle *>(col);
			if (Collision2::CheckCircleBox(*sphere, *box)) {
				return true;
			}
		}
		else if (col->GetShapeType() == COL_SHAPE_BOX &&
			colB->GetShapeType() == COL_SHAPE_BOX &&
			colB->GetTag() == Tag) {
			Box *boxA = dynamic_cast<Box *>(col);
			Box *boxB = dynamic_cast<Box *>(colB);
			if (Collision2::CheckBoxBox(*boxA, *boxB)) {
				return true;
			}
		}
		else if (col->GetShapeType() == COL_SHAPE_POINT &&
			colB->GetShapeType() == COL_SHAPE_BOX &&
			colB->GetTag() == Tag) {
			Point *point = dynamic_cast<Point *>(col);
			Box *box = dynamic_cast<Box *>(colB);
			if (Collision2::CheckBoxPoint(*box, *point)) {
				return true;
			}
		}
		else if (col->GetShapeType() == COL_SHAPE_BOX &&
			colB->GetShapeType() == COL_SHAPE_POINT &&
			colB->GetTag() == Tag) {
			Box *box = dynamic_cast<Box *>(col);
			Point *point = dynamic_cast<Point *>(colB);
			if (Collision2::CheckBoxPoint(*box, *point)) {
				return true;
			}
		}
		else if (col->GetShapeType() == COL_SHAPE_POINT &&
			colB->GetShapeType() == COL_SHAPE_CIRCLE &&
			colB->GetTag() == Tag) {
			Point *point = dynamic_cast<Point *>(col);
			Circle *circle = dynamic_cast<Circle *>(colB);
			if (Collision2::CheckCirclePoint(*circle, *point)) {
				return true;
			}
		}
		else if (col->GetShapeType() == COL_SHAPE_CIRCLE &&
			colB->GetShapeType() == COL_SHAPE_POINT &&
			colB->GetTag() == Tag) {
			Circle *circle = dynamic_cast<Circle *>(col);
			Point *point = dynamic_cast<Point *>(colB);
			if (Collision2::CheckCirclePoint(*circle, *point)) {
				return true;
			}
		}
	}
	return false;
}
