#pragma once
#ifndef	TRANSFORM_H
#define TRANSFORM_H

#include <Maths/Matrix3D.h>
#include "ECSComponent.h"

namespace BE
{
	class Transform : public IECSComponent
	{
		//World position, rotation, scale of object
		Vec3f m_position;
		Vec3f m_scale;
		Quatf m_rotation;

		//Local position, rotation, scale of object relative to the parent, or world if no parent
		Vec3f m_localPosition;
		Vec3f m_localScale;
		Quatf m_localRotation;

		//Children entities under this entity
		std::vector<EntityID> m_children;

		//The parent entity of this entity
		EntityID m_parent;


	public:
		Transform();
		Transform(const Transform& rhs);
		Transform(Transform&& rhs);
		Transform& operator=(const Transform& rhs);
		Transform& operator=(Transform&& rhs);


		/*
			Position
		*/
		//Get world position of the entity
		inline const Vec3f& GetPositon() const					{ return m_position; }

		//Get local position relative to the parent
		inline const Vec3f& GetLocalPosition() const			{ return m_localPosition; }

		//Set world position for this entity
		void SetPosition(const Vec3f& _worldPos);

		//Set local position relative to the parent
		void SetLocalPosition(const Vec3f& _localPos);



		/*
			Scale
		*/
		//Get world scale of the entity
		inline const Vec3f& GetScale() const					{ return m_scale; }

		//Get local scale relative to the parent
		inline const Vec3f& GetLocalScale() const				{ return m_localScale; }

		//Set world scale for this entity
		void SetScale(const Vec3f& _worldScale);

		//Set local scale relative to the parent
		void SetLocalScale(const Vec3f& _localScale);



		/*
			Rotation
		*/
		//Get world rotation of the entity
		inline const Quatf& GetRotation() const									{ return m_rotation; }

		//Get local rotation relative to the parent
		inline const Quatf& GetLocalRotation() const							{ return m_localRotation; }

		//Get world rotation of the entity as euler angles (Degrees)
		inline Vec3f GetRotation_AsEuler() const								{ return GetRotation().GetEulerAngles(); }

		//Get local rotation relative to the parent as euler angles
		inline Vec3f GetLocalRotation_AsEuler() const							{ return GetLocalRotation().GetEulerAngles(); }

		//Set world rotation for this entity
		void SetRotation(const Quatf& _worldRot);

		//Set local rotation relative to the parent
		void SetLocalRotation(const Quatf& _localRotation);
		
		//Set world rotation for this entity using euler angles (Degrees)
		inline void SetRotation_AsEuler(const Vec3f& _worldRot)					{ SetRotation(Quatf(_worldRot.x, _worldRot.y, _worldRot.z)); }

		//Set local rotation relative to the parent using euler angles
		inline void SetLocalRotation_AsEuler(const Vec3f& _localRotation)		{ SetLocalRotation(Quatf(_localRotation.x, _localRotation.y, _localRotation.z)); }
		//ONLY TO BE USED BY RTTR, EVERYONE ELSE PASS BY REFERENCE
		inline void SetLocalRotation_AsEulerValue(const Vec3f& _localRotation) { SetLocalRotation_AsEulerValue(_localRotation);}
		inline void SetRotation_AsEulerValue(const Vec3f _worldRot)				{ SetRotation_AsEuler(_worldRot); }


		/*
			Parent-child hierachy
		*/
		//Attaches a child to this entity. 
		//MUST PUT _thisEntityID AS THE OWNER ENTITY ID OF THIS TRANSFORM!!!!!!!
		void AttachChild(EntityID _thisEntityID, EntityID _childID);

		//Detaches a child from this entity
		void DetachChild(EntityID _childID);

		//Get all the children attached to this entity
		inline const auto& GetChildren() const							{ return m_children; }

		//Number of children this entity has
		inline size_t GetChildrenCount() const							{ return m_children.size(); }

		//Check if this entity has a specified child
		inline bool HasChild(EntityID _childID) const					{ return std::find(m_children.begin(), m_children.end(), _childID) != m_children.end(); }

		//Finds the parent of this entity, NULL_ENTITY if no parent
		inline EntityID GetParent() const								{ return m_parent; }

		//Whether this entity has parents
		inline bool HasParent() const									{ return m_parent != NULL_ENTITY; }



		/*
			Serialisation
		*/
		void LoadObject(const rapidjson::Value& v) override;
		void SaveObject(rapidjson::Value&, rapidjson::Document&) override;
#if 1
		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
#endif
	};

	/*
		TO DO: if this entity is going to be deleted, must remove this entity from m_parent of all its children,
		and remove this entity from m_children from its parent. Do so before actually removing data.
		Do this in class ECS in DestroyEntity
	*/

}


#endif