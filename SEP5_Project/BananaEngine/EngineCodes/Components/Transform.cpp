#include "pch.h"
#include "Transform.h"
#include <Systems/ECS/ECS.h>
#include <Serialisation/BananaSerialisation.h>
namespace BE
{
	Transform::Transform()
		: m_position{}, m_scale{ 1.0f, 1.0f, 1.0f }, m_rotation{},
		m_localPosition{}, m_localScale{ 1.0f, 1.0f, 1.0f }, m_localRotation{},
		m_children{}, m_parent{ NULL_ENTITY }
	{

	}

	Transform::Transform(const Transform& rhs)
		: m_position{ rhs.m_position }, m_scale{ rhs.m_scale }, m_rotation{ rhs.m_rotation },
		m_localPosition{ rhs.m_position }, m_localScale{ rhs.m_scale }, m_localRotation{ rhs.m_rotation },
		m_children{}, m_parent{ NULL_ENTITY }
	{
		//Do not copy the children and the parent - will result in duplicated UUID
		//Local Pos/Rot/Scale will just be Global Pos/Rot/Scale because no parent
	}

	Transform::Transform(Transform&& rhs)
		: m_position{ rhs.m_position }, m_scale{ rhs.m_scale }, m_rotation{ rhs.m_rotation },
		m_localPosition{ rhs.m_localPosition }, m_localScale{ rhs.m_localScale }, m_localRotation{ rhs.m_localRotation },
		m_children{ std::move(rhs.m_children) }, m_parent{ rhs.m_parent }
	{
		rhs.m_position = Vec3<float>{};
		rhs.m_scale = Vec3<float>{};
		rhs.m_rotation = Quaternion<float>{};
		rhs.m_localPosition = Vec3<float>{};
		rhs.m_localScale = Vec3<float>{};
		rhs.m_localRotation = Quaternion<float>{};
		rhs.m_parent = NULL_ENTITY;
	}

	Transform& Transform::operator=(const Transform& rhs)
	{
		//Do not copy the children and the parent - will result in duplicated UUID
		//Local Pos/Rot/Scale will just be Global Pos/Rot/Scale because no parent
		m_position = rhs.m_position;
		m_scale = rhs.m_scale;
		m_rotation = rhs.m_rotation;

		m_localPosition = rhs.m_position;
		m_localScale = rhs.m_scale;
		m_localRotation = rhs.m_rotation;

		m_parent = NULL_ENTITY;
		m_children.clear();

		return *this;
	}

	Transform& Transform::operator=(Transform&& rhs)
	{
		m_position = rhs.m_position;
		m_scale = rhs.m_scale;
		m_rotation = rhs.m_rotation;

		m_localPosition = rhs.m_localPosition;
		m_localScale = rhs.m_localScale;
		m_localRotation = rhs.m_localRotation;

		m_parent = rhs.m_parent;
		m_children = std::move(rhs.m_children);

		rhs.m_position = Vec3<float>{};
		rhs.m_scale = Vec3<float>{};
		rhs.m_rotation = Quaternion<float>{};
		rhs.m_localPosition = Vec3<float>{};
		rhs.m_localScale = Vec3<float>{};
		rhs.m_localRotation = Quaternion<float>{};
		rhs.m_parent = NULL_ENTITY;

		return *this;
	}




	//Set world position for this entity
	void Transform::SetPosition(const Vec3f& _worldPos)
	{
		m_position = _worldPos;

		//If entity does not have parent, then set local position as world position as well
		if (!this->HasParent())
		{
			m_localPosition = _worldPos;
		}
		else
		{
			//Get the parent's transform component
			const Vec3f& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();
			const Vec3f& parentPos = ECS->GetComponent<Transform>(m_parent).GetPositon();
			const Quatf& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//Reverse scale -> rotation -> translation
			// (TRS)^-1 = S^-1 * R^-1 * T^-1

			//Directly find the inverse of each transform component
			Mat3f invScale = Mat3f{}.SetScale(1 / parentScale.x, 1 / parentScale.y, 1 / parentScale.z);
			Mat3f invRot = Mat3f{}.SetRotation(parentRot).SetTranspose();
			//For invRot, because columns in rotation matrix is orthonormal, we can just transpose to find inverse

			//Find the offset between this object and parent. Parent's position will be the local origin
			Vec3f offset = parentPos.VecTowards(this->m_position);

			//Result is the entity's local position using parent as the coordinate system (accounts for scale and rotation)
			m_localPosition = invScale * (invRot * offset);
		}

		//Also update children's transform
		for (EntityID& e : m_children)
		{
			ECS->GetComponent<Transform>(e).SetLocalPosition(ECS->GetComponent<Transform>(e).GetLocalPosition());
		}
	}

	//Set local position relative to the parent
	void Transform::SetLocalPosition(const Vec3f& _localPos)
	{
		m_localPosition = _localPos;

		if (!this->HasParent())
		{
			//If this entity has no parent, then set local position as world position
			m_position = _localPos;
		}
		else
		{
			//Get the parent's transform component
			const Vec3f& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();
			const Vec3f& parentPos = ECS->GetComponent<Transform>(m_parent).GetPositon();
			const Quatf& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//Get the transformation matrices
			Mat3f scale = Mat3f{}.SetScale(parentScale.x, parentScale.y, parentScale.z);
			Mat3f rot = Mat3f{}.SetRotation(parentRot);

			//Result is the conversion of the given local position relative to it's parent to global position
			m_position = parentPos + (rot * (scale * _localPos));
		}

		//Also update children transform
		for (EntityID& e : m_children)
		{
			ECS->GetComponent<Transform>(e).SetLocalPosition(ECS->GetComponent<Transform>(e).GetLocalPosition());
		}
	}

	//Set world scale for this entity
	void Transform::SetScale(const Vec3f& _worldScale)
	{
		m_scale = _worldScale;

		if (!this->HasParent())
		{
			//If this entity has no parent, then set local scale as world scale
			m_localScale = _worldScale;
		}
		else
		{
			//Get the parent's global scale
			const Vec3f& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();

			//This entity's local scale relative to the parent's scale (Parent's scale is like Vec3(1,1,1))
			m_localScale = Vec3f(m_scale.x / parentScale.x,
				m_scale.y / parentScale.y,
				m_scale.z / parentScale.z);
		}

		//Also update children's transform
		for (EntityID& e : m_children)
		{
			ECS->GetComponent<Transform>(e).SetLocalScale(ECS->GetComponent<Transform>(e).GetLocalScale());
			ECS->GetComponent<Transform>(e).SetLocalPosition(ECS->GetComponent<Transform>(e).GetLocalPosition());
		}
	}

	//Set local scale relative to the parent
	void Transform::SetLocalScale(const Vec3f& _localScale)
	{
		m_localScale = _localScale;

		if (!this->HasParent())
		{
			//If this entity has no parent, then set local scale as world scale
			m_scale = _localScale;
		}
		else
		{
			//Get the parent's global scale
			const Vec3f& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();

			//This entity's local scale relative to the parent's scale (Parent's scale is like Vec3(1,1,1))
			m_scale = Vec3f(_localScale.x * parentScale.x,
				_localScale.y * parentScale.y,
				_localScale.z * parentScale.z);
		}

		//Also update children's transform
		for (EntityID& e : m_children)
		{
			ECS->GetComponent<Transform>(e).SetLocalScale(ECS->GetComponent<Transform>(e).GetLocalScale());
			ECS->GetComponent<Transform>(e).SetLocalPosition(ECS->GetComponent<Transform>(e).GetLocalPosition());
		}
	}

	//Set world rotation for this entity
	void Transform::SetRotation(const Quatf& _worldRot)
	{
		m_rotation = _worldRot;

		if (!this->HasParent())
		{
			//If this entity has no parent, then set local rotation as world rotation
			m_localRotation = _worldRot;
		}
		else
		{
			//Get the parent's global rotation
			const Quatf& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//This entity's local rotation relative to the parent's rotation (Parent's rotation is like no rotation)
			m_localRotation = m_rotation * parentRot.Inverse();
		}

		//Also update children's transform
		for (EntityID& e : m_children)
		{
			ECS->GetComponent<Transform>(e).SetLocalRotation(ECS->GetComponent<Transform>(e).GetLocalRotation());
			ECS->GetComponent<Transform>(e).SetLocalPosition(ECS->GetComponent<Transform>(e).GetLocalPosition());
		}
	}

	//Set local rotation relative to the parent
	void Transform::SetLocalRotation(const Quatf& _localRotation)
	{
		m_localRotation = _localRotation;

		if (!this->HasParent())
		{
			//If this entity has no parent, then set local rotation as world rotation
			m_rotation = _localRotation;
		}
		else
		{
			//Get the parent's global scale
			const Quatf& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//This entity's local rotation relative to the parent's rotation (Parent's rotation is like no rotation)
			m_rotation = _localRotation * parentRot;
		}

		//Also update children's transform
		for (EntityID& e : m_children)
		{
			ECS->GetComponent<Transform>(e).SetLocalRotation(ECS->GetComponent<Transform>(e).GetLocalRotation());
			ECS->GetComponent<Transform>(e).SetLocalPosition(ECS->GetComponent<Transform>(e).GetLocalPosition());
		}
	}



	/*
	Vec3<float> Transform::GetLocalPosition() const
	{
		if (!this->HasParent())
		{
			//If this entity has no parent, then return as world position
			return m_position;
		}
		else
		{
			//Get the parent's transform component
			const Vec3<float>& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();
			const Vec3<float>& parentTrans = ECS->GetComponent<Transform>(m_parent).GetPositon();
			const Quaternion<float>& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//Reverse scale -> rotation -> translation
			// (TRS)^-1 = S^-1 * R^-1 * T^-1

			//Directly find the inverse of each transform component
			Mat3<float> invScale = Mat3<float>{}.SetScale(1.0f / parentScale.x, 1.0f / parentScale.y, 1.0f / parentScale.z);
			Mat3<float> invRot = Mat3<float>{}.SetRotation(parentRot).SetTranspose();
			//For invRot, because columns in rotation matrix is orthonormal, we can just transpose to find inverse

			//Find the offset between this object and parent. Parent's position will be the local origin
			Vec3<float> offset = parentTrans.VecTowards(this->m_position);

			//Result is the entity's local position using parent as the coordinate system (accounts for scale and rotation)
			return invScale * (invRot * offset);

		}
	}

	void Transform::SetLocalPosition(const Vec3<float>& _localPos)
	{
		if (!this->HasParent())
		{
			//If this entity has no parent, then set local position as world position
			m_position = _localPos;
		}
		else
		{
			//Get the parent's transform component
			const Vec3<float>& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();
			const Vec3<float>& parentTrans = ECS->GetComponent<Transform>(m_parent).GetPositon();
			const Quaternion<float>& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//Get the transformation matrices
			Mat3<float> scale = Mat3<float>{}.SetScale(parentScale.x, parentScale.y, parentScale.z);
			Mat3<float> rot = Mat3<float>{}.SetRotation(parentRot);

			//Result is the conversion of the given local position relative to it's parent to global position
			m_position = parentTrans + (rot * (scale * _localPos));
		}
	}



	Vec3<float> Transform::GetLocalScale() const
	{
		if (!this->HasParent())
		{
			//If this entity has no parent, then return as world scale
			return m_scale;
		}
		else
		{
			//Get the parent's global scale
			const Vec3<float>& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();

			//This entity's local scale relative to the parent's scale (Parent's scale is like Vec3(1,1,1))
			return Vec3<float>(m_scale.x / parentScale.x,
				m_scale.y / parentScale.y,
				m_scale.z / parentScale.z);
		}
	}

	void Transform::SetLocalScale(const Vec3<float>& _localScale)
	{
		if (!this->HasParent())
		{
			//If this entity has no parent, then set local scale as world scale
			m_scale = _localScale;
		}
		else
		{
			//Get the parent's global scale
			const Vec3<float>& parentScale = ECS->GetComponent<Transform>(m_parent).GetScale();

			//This entity's local scale relative to the parent's scale (Parent's scale is like Vec3(1,1,1))
			m_scale = Vec3<float>(_localScale.x * parentScale.x,
				_localScale.y * parentScale.y,
				_localScale.z * parentScale.z);
		}
	}



	Quaternion<float> Transform::GetLocalRotation() const
	{
		if (!this->HasParent())
		{
			//If this entity has no parent, then return as world rotation
			return m_rotation;
		}
		else
		{
			//Get the parent's global rotation
			const Quaternion<float>& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//This entity's local rotation relative to the parent's rotation (Parent's rotation is like no rotation)
			return m_rotation * parentRot.Inverse();
		}
	}

	void Transform::SetLocalRotation(const Quaternion<float>& _localRotation)
	{
		if (!this->HasParent())
		{
			//If this entity has no parent, then set local rotation as world rotation
			m_rotation = _localRotation;
		}
		else
		{
			//Get the parent's global scale
			const Quaternion<float>& parentRot = ECS->GetComponent<Transform>(m_parent).GetRotation();

			//This entity's local rotation relative to the parent's rotation (Parent's rotation is like no rotation)
			m_rotation = _localRotation * parentRot;
		}
	}

	Vec3<float> Transform::GetLocalRotation_AsEuler() const
	{
		return this->GetLocalRotation().GetEulerAngles();
	}

	void Transform::SetLocalRotation_AsEuler(const Vec3<float>& _localRotation)
	{
		this->SetLocalRotation(Quaternion<float>(_localRotation.x, _localRotation.y, _localRotation.z));
	}
*/


	void Transform::AttachChild(EntityID _thisEntityID, EntityID _childID)
	{
		//Cannot attach child to itself
		if (_thisEntityID == _childID)
		{
			LogCoreError("Cannot attach child to itself");
			return;
		}

		//If _thisEntityID already has _childID in the children list, then don't do anything
		if (HasChild(_childID) && ECS->GetComponent<Transform>(_childID).GetParent() == _thisEntityID)
			return;

		//If child contains a parent, then detach it first
		if (ECS->GetComponent<Transform>(_childID).HasParent())
		{
			//Get the old parent
			EntityID oldParentID = ECS->GetComponent<Transform>(_childID).GetParent();

			//Make the old parent detach the child
			ECS->GetComponent<Transform>(oldParentID).DetachChild(_childID);
		}

		//Attach this child to the new parent
		m_children.push_back(_childID);

		//Update the child's parent value
		ECS->GetComponent<Transform>(_childID).m_parent = _thisEntityID;

		//Now need to update the local transform of the child
		//The global transform shouldn't change, so we'll get local transform via the global functions
		ECS->GetComponent<Transform>(_childID).SetPosition(ECS->GetComponent<Transform>(_childID).GetPositon());
		ECS->GetComponent<Transform>(_childID).SetScale(ECS->GetComponent<Transform>(_childID).GetScale());
		ECS->GetComponent<Transform>(_childID).SetRotation(ECS->GetComponent<Transform>(_childID).GetRotation());
	}

	void Transform::DetachChild(EntityID _childID)
	{
		auto it = std::find(m_children.begin(), m_children.end(), _childID);

		//If this entity contains this child
		if (it != m_children.end())
		{
			//Set this child's parent to be NULL
			ECS->GetComponent<Transform>(_childID).m_parent = NULL_ENTITY;

			//Remove this child from this entity's children list
			m_children.erase(it);
		}

		//Now need to update the local transform of the child
		//Because child is parentless, the local transform is now global transform
		ECS->GetComponent<Transform>(_childID).SetPosition(ECS->GetComponent<Transform>(_childID).GetPositon());
		ECS->GetComponent<Transform>(_childID).SetScale(ECS->GetComponent<Transform>(_childID).GetScale());
		ECS->GetComponent<Transform>(_childID).SetRotation(ECS->GetComponent<Transform>(_childID).GetRotation());
	}



	void Transform::LoadObject(const rapidjson::Value&)
	{

	}

	void Transform::SaveObject(rapidjson::Value&, rapidjson::Document&)
	{

	}
#if 1
	RTTR_REGISTRATION
	{
		using namespace rttr;
		using namespace BE::Serialisation;
		registration::class_<Transform>(typeid(Transform).name())
		.constructor()
		(
		)
		.property("m_position", &Transform::GetPositon, &Transform::SetPosition)
		(
			metadata(MetaDataKey::Serialize,true),
			metadata(MetaDataKey::Copy,true)
		)
		.property("m_localposition", &Transform::GetLocalPosition, &Transform::SetLocalPosition)
		(
			metadata(MetaDataKey::Serialize, false),
			metadata(MetaDataKey::Copy, false)
		)
		.property("m_scale", &Transform::GetScale, &Transform::SetScale)
		(
			metadata(MetaDataKey::Serialize, true),
			metadata(MetaDataKey::Copy, true)
		)
		.property("m_localscale", &Transform::GetLocalScale, &Transform::SetLocalScale)
		(
			metadata(MetaDataKey::Serialize, false),
			metadata(MetaDataKey::Copy, false)
		)
		.property("m_rotation", &Transform::GetRotation, &Transform::SetRotation)
		(
			metadata(MetaDataKey::Serialize, false),
			metadata(MetaDataKey::Copy, false)
		)
		.property("m_localrotation", &Transform::GetLocalRotation, &Transform::SetLocalRotation)
		(
			metadata(MetaDataKey::Serialize, false),
			metadata(MetaDataKey::Copy, false)
		)
		.property("m_eulerrotation", &Transform::GetRotation_AsEuler, &Transform::SetRotation_AsEulerValue)
		(
			metadata(MetaDataKey::Serialize, false),
			metadata(MetaDataKey::Copy, false)
		);

	}
#endif
}