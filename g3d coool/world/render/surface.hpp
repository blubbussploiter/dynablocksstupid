#pragma once

#include "renderHelp.hpp"
#include "cylinder.hpp"

#define bump_pad 0.5f
#define bump 0.15f

static void build_simple_bump(block::Block* self, const Vector3& size, const CoordinateFrame& offset)
{
	build_simple_face(self, block::TOP, size, offset);
	build_simple_face(self, block::FRONT, size, offset);
	build_simple_face(self, block::BACK, size, offset);
	build_simple_face(self, block::RIGHT, size, offset);
	build_simple_face(self, block::LEFT, size, offset);
}

static void add_torq_surface(block::Block* self, const Vector3& size, block::Controller::ControllerType control, block::Face face, int hinge)
{
	CoordinateFrame origin;
	switch (face)
	{
		case block::TOP:
		{
			origin = Vector3(0, size.y, 0);
			break;
		}
		case block::BOTTOM:
		{
			origin = Vector3(0, -size.y, 0);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(-180), 0, 0);
			break;
		}
		case block::LEFT:
		{
			origin = Vector3(-size.x, 0, 0);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(90));
			break;
		}
		case block::RIGHT:
		{
			origin = Vector3(size.x, 0, 0);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(-90));
			break;
		}
		case block::FRONT:
		{
			origin = Vector3(0, 0, -size.z);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(-90), 0, toRadians(0));
			break;
		}
		case block::BACK:
		{
			origin = Vector3(0, 0, size.z);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(0));
			break;
		}
	}
	if (hinge)
	{
		build_surface_cylinder(self, origin, Color3::yellow(), 0.25f, 0.1f, 6);
	}
	else
	{
		Color3 baseColor = Color3::gray();
		switch (control)
		{
			case block::Controller::PRIMARY_CONTROLLER:
			{
				baseColor = Color3::red();
				break;
			}
			case block::Controller::SECONDARY_CONTROLLER:
			{
				baseColor = Color3::blue();
				break;
			}
			case block::Controller::AI_CHASE_CONTROLLER:
			{
				baseColor = Color3::black();
				break;
			}
			case block::Controller::AI_FLEE_CONTROLLER:
			{
				baseColor = Color3::yellow();
				break;
			}
		}
		build_surface_cylinder(self, origin, Color3::yellow(), 0.2f, 0.075f, 6);
		build_surface_cylinder(self, origin, baseColor, 0.05f, 0.15f, 6);
	}
}

static void add_bumps(block::Block* self, const Vector3& size, block::Face face)
{
	CoordinateFrame origin;
	Vector3 axis_hors;
	Vector3 axis_vert;
	int x = 0;
	int y = 0;
	Vector3 realSize = size * 4;
	switch (face)
	{
	case block::TOP:
	case block::BOTTOM:
	{
		x = realSize.x;
		y = realSize.z;
		axis_hors = Vector3(1, 0, 0);
		axis_vert = Vector3(0, 0, 1);
		if (face == block::TOP)
		{
			origin = Vector3(-size.x + 0.25f, size.y, -size.z + 0.25f);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(0, 0, 0);
			break;
		}
		else
		{
			origin = Vector3(-size.x + 0.25f, -size.y, -size.z + 0.25f);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(180), 0, 0);
			break;
		}
		break;
	}
	case block::LEFT:
	case block::RIGHT:
	{
		x = realSize.z;
		y = realSize.y;
		axis_hors = Vector3(0, 0, 1);
		axis_vert = Vector3(0, 1, 0);
		if (face == block::RIGHT)
		{
			origin = Vector3(size.x, -size.y + 0.25f, -size.z + 0.25f);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(-90));
			break;
		}
		else
		{
			origin = Vector3(-size.x, -size.y + 0.25f, -size.z + 0.25f);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(-90), 0, toRadians(90));
			break;
		}
		break;
	}
	case block::FRONT:
	case block::BACK:
	{
		x = realSize.x;
		y = realSize.y;
		axis_hors = Vector3(1, 0, 0);
		axis_vert = Vector3(0, 1, 0);
		if (face == block::FRONT)
		{
			origin = Vector3(-size.x + 0.25f, -size.y + 0.25f, -size.z);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(-90), 0, toRadians(0));
			break;
		}
		else
		{
			origin = Vector3(-size.x + 0.25f, -size.y + 0.25f, size.z);
			origin.rotation = Matrix3::fromEulerAnglesXYZ(toRadians(90), 0, toRadians(0));
			break;
		}
		break;
	}
	}

	/* ezpz: bottle neck right here */
	int lastsize = self->part_indices.size();
	for (int i = 0; i < y; i++)
	{
		Vector3 vert = (axis_vert * (i * bump_pad));

		build_simple_bump(self,
			Vector3(bump, 0.05f, bump),
			origin + vert);
		for (int o = 1; o < x; o++)
		{
			Vector3 hors = (axis_hors * (o * (bump_pad)));
			build_simple_bump(self,
				Vector3(bump, 0.05f, bump),
				origin + vert + hors);
		}
	}
	printf("added %d vertices\n", self->part_indices.size() - lastsize);
}
