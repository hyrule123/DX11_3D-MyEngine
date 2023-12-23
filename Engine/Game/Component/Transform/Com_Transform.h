#pragma once
#include "iTransform.h"



#include "../../../DefaultShader/CommonStruct.hlsli"

namespace ehw
{
    class GameObject;
    class Com_Transform :
        public iTransform
    {
    public:
        Com_Transform();

        //단순 Value만 저장 중이므로 기본 복사 생성자로도 충분함.
        Com_Transform(const Com_Transform& _other) = default;
        CLONE(Com_Transform);

        virtual ~Com_Transform();

        virtual eResult SaveJson(Json::Value* _pJson) override;
        virtual eResult LoadJson(const Json::Value* _pJson) override;

    public:
        //virtual void Init() override;
        virtual void InternalUpdate() override;
        virtual void BindData() override; 

    public:
        //inline Setter

        //사이즈의 경우 전체를 갱신할 필요 없음.
        void SetSize(const float3& _vSize);
        void SetSizeXY(const float2& _v2Size);

        void SetRelativePos(const float3& _v3Pos) { mPosRelative = _v3Pos; SetMyUpdate(); }
        void SetRelativePosXY(const float2& _v2Pos) { mPosRelative.x = _v2Pos.x; mPosRelative.y = _v2Pos.y; SetMyUpdate(); }
        void SetRelativePos(float _x, float _y, float _z) { mPosRelative = float3(_x, _y, _z); SetMyUpdate(); }
        void AddRelativePos(const float3& _v3Pos) { mPosRelative += _v3Pos; SetMyUpdate(); }
        void AddRelativePosXY(const float2& _v2Pos) { mPosRelative += _v2Pos; SetMyUpdate(); }


        void SetRelativeScale(const float3& _vScale);
        void SetRelativeScale(float _x, float _y, float _z);

        void SetRelativeRotXYZ(const float3& _vRot) { mRotRelative = _vRot; SetMyUpdate(); }
        void SetRelativeRotXYZ(float _x, float _y, float _z) { mRotRelative = float3(_x, _y, _z); SetMyUpdate(); }
        void SetRelativeRotX(float _x) { mRotRelative.x = _x; SetMyUpdate(); }
        void SetRelativeRotY(float _y) { mRotRelative.y = _y; SetMyUpdate(); }
        void SetRelativeRotZ(float _z) { mRotRelative.z = _z; SetMyUpdate(); }

        void SetRelativePosX(float _x) { mPosRelative.x = _x; SetMyUpdate(); }
        void SetRelativePosY(float _y) { mPosRelative.y = _y; SetMyUpdate(); }
        void SetRelativePosZ(float _z) { mPosRelative.z = _z; SetMyUpdate(); }

        void SetScaleInheritance(bool _bInherit) { mbInheritScale = _bInherit; SetMyUpdate(); }
        void SetRotInheritance(bool _bInherit) { mbInheritRot = _bInherit; SetMyUpdate(); }

        //상대 회전값은 변화하지만 실제로 회전하지는 않음.
        void SetLockRotation(bool _bLockRot) { mbLockRotation = _bLockRot; }

        //이번 틱에 업데이트를 해야한다고 설정. 자신의 움직임에 영향을 받는 자식 오브젝트들에게도 재귀적으로 알림
        void SetMyUpdate() { mbNeedMyUpdate = true; }


        //이 길이는 트랜스폼을 정사각형(또는 정육면체)라고 가정했을 때 한 모서리까지의 길이이다.
        //이 길이를 간이 충돌체 한 변의 반의 길이로 사용하면, 
        //어떤 도형이던 간에 안에 들어오는 형태의 정사각형 또는 정육면체를 만들 수 있다.(간이 충돌체로 적합)
        //사이즈 또는 스케일값이 변했을 경우 간이 충돌체 정보를 새로 생성.
        //현재 컬링 및 간이 충돌체 계산에 사용 중.
        bool IsUpdated() const { return mbNeedMyUpdate; }
        bool GetSizeUpdated() const { return mbSizeUpdated; }

        //inline Getter
        const float3& GetSize() const { return mSize; }

        //개별 사이즈 + 월드 Scale 반영된 실제 사이즈
        float3 GetWorldSize() const;

        const float3& GetRelativePos() const { return mPosRelative; }
        float3 GetWorldPos() const { return float3(mMatRelative.m[3]); }
        float GetWorldPosX() const { return mCB_Transform.World.m[3][0]; }
        float GetWorldPosY() const { return mCB_Transform.World.m[3][1]; }
        float GetWorldPosZ() const { return mCB_Transform.World.m[3][2]; }
        const float3& GetRelativeScale() const { return mScaleRelative; }
        float3 GetWorldScale() const;

        float GetRelativeRotX() const { return mRotRelative.x; }
        float GetRelativeRotY() const { return mRotRelative.y; }
        float GetRelativeRotZ() const { return mRotRelative.z; }
        const float3& GetRelativeRotXYZ() const { return mRotRelative; }


        MATRIX GetWorldRotMat() const;
        float3 GetWorldRot(eAxis3D _eAxis) const;

        const float3& GetRelativeDir(eDirectionType _eDir) const { return mDirRelative[(UINT)_eDir]; }
        const float3& Forward() const { return mDirRelative[(int)eDirectionType::FRONT]; }
        const float3& Up() const { return mDirRelative[(int)eDirectionType::UP]; }
        const float3& Right() const { return mDirRelative[(int)eDirectionType::RIGHT]; }
        float3 GetWorldDir(eDirectionType _eDir) const { return float3((float*)mMatRelative.m[(UINT)_eDir]).Normalize(); }
        const MATRIX& GetWorldMat() const { return mCB_Transform.World; }


        //호출 시점: GameObject에서 FinalTick() 순회 끝난 이후
        //갱신 여부를 전부 끔
        void ClearUpdateState();
    private:

        //Scale 적용 안된 단순 bottom - y sorting 용도(OBB 등에는 사용하지 말것)
        float3 GetSimpleBottom() const { return GetWorldPos() - mSize; }

        void UpdateMyTransform();

        //부모의 트랜스폼 행렬을 받아서 최종적인 월드행렬을 업데이트 한다.
        void UpdateParentMatrix(const std::shared_ptr<Com_Transform>& _parentTransform);

    private:
        float3    mSize;
        float3    mScaleRelative;

        float3    mPosRelative;
        float3    mRotRelative;

        //앞, 위, 오른쪽으로 나타내는 직관적인 방향 정보
        //eDirectionType 열거체를 사용.
        float3    mDirRelative[(int)eDirectionType::END];

        //월드 방향(모든 회전정보 누적)
        float3    mDirWorld[(int)eDirectionType::END];

        //상속받지 않은 고유 트랜스폼. Scale은 적용, Size는 미적용
        MATRIX  mMatRelative;

        //부모로부터 누적된 트랜스폼 정보
        MATRIX  mMatParent;

        //최종 행렬은 여기에 저장됨
        tCB_Transform mCB_Transform;

        bool    mbInheritScale;
        bool    mbInheritRot;
        bool    mbLockRotation; //자신의 회전 방지

        //상속 형태로 업데이트가 필요한지 여부를 저장.
        //위치가 변하지 않았는데 굳이 월드행렬을 업데이트 할 필요가 없음.
        //mMatRelative를 업데이트 해야하는지 여부가 저장되어있음.
        //자신의 행렬을 업데이트 해야 한다면 반드시 부모 행렬을 받아와야 하기 때문에 m_bNeedParentUpdate도 업데이트 해준다.
        bool    mbNeedMyUpdate;

        //Size는 자신에게만 적용되는 고유값이므로 재귀형으로 전달할 필요 없음.
        bool    mbSizeUpdated;
    };


    inline void Com_Transform::SetSize(const float3& _vSize)
    {
        mSize = _vSize;
        mbSizeUpdated = true;
    }

    inline void Com_Transform::SetSizeXY(const float2& _v2Size)
    {
        mSize = _v2Size;
        mbSizeUpdated = true;
    }

    inline void Com_Transform::SetRelativeScale(const float3& _vScale)
    {
        mScaleRelative = _vScale;
        SetMyUpdate();
        mbSizeUpdated = true;
    }

    inline void Com_Transform::SetRelativeScale(float _x, float _y, float _z)
    {
        mScaleRelative = float3(_x, _y, _z);
        SetMyUpdate();
        mbSizeUpdated = true;
    }


    inline float3 Com_Transform::GetWorldSize() const
    {
        return float3(mCB_Transform.World.Right().Length(), mCB_Transform.World.Up().Length(), mCB_Transform.World.Forward().Length());
    }

    inline float3 Com_Transform::GetWorldScale() const
    {
        return float3(mCB_Transform.World.Right().Length(), mCB_Transform.World.Right().Length(), mCB_Transform.World.Right().Length());
    }

    inline MATRIX Com_Transform::GetWorldRotMat() const
    {
        return MATRIX(mCB_Transform.World.Right().Normalize(), mCB_Transform.World.Up().Normalize(), mCB_Transform.World.Forward().Normalize());
    }

    inline float3 Com_Transform::GetWorldRot(eAxis3D _eAxis) const
    {
        return mCB_Transform.World.Axis((eAxis4D)_eAxis).Normalize();
    }

    inline void Com_Transform::ClearUpdateState()
    {
        mbNeedMyUpdate = false;
        mbSizeUpdated = false;
    }

}
