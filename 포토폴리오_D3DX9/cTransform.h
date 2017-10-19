#pragma once

class NxVec3;

class cTransform
{
protected:

	SYNTHESIZE(D3DXVECTOR3, m_position, Position);
	SYNTHESIZE(D3DXVECTOR3, m_size, Size);
	SYNTHESIZE(D3DXVECTOR3, m_direction, Direction);
	D3DXQUATERNION m_Quaternion;
	float	m_Yam;
	float	m_Pitch;
	float	m_Roll;

	bool m_PitchOver;

	D3DXVECTOR3 m_top;

	D3DXVECTOR3 front;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;


	D3DXMATRIXA16 m_matWorld;

	bool DebugTr;
public:

	cTransform();
	~cTransform();

	cTransform* GetTransform() { return this; }
	void SetTransform(cTransform* Tr)
	{
		SetPosition(Tr->GetPosition());
		SetSize(Tr->GetSize());
		m_Quaternion = Tr->m_Quaternion;
		m_Yam = Tr->m_Yam;
		m_Pitch = Tr->m_Pitch;
		m_Roll = Tr->m_Roll;
		m_PitchOver = Tr->m_PitchOver;
		m_top = Tr->m_top;
		front = Tr->front;
		right = Tr->right;
		up = Tr->up;
		m_matWorld = Tr->m_matWorld;
		DebugTr = Tr->DebugTr;
	}

	D3DXVECTOR3 GetDirection();

	D3DXVECTOR3 GetFrontVec();
	D3DXVECTOR3 GetRightVec();
	D3DXVECTOR3 GetUpVec();

	D3DXQUATERNION GetQuaternion(D3DXQUATERNION* qua = NULL) {
		if (qua) { qua = &m_Quaternion; return D3DXQUATERNION(); }
		else { return m_Quaternion; }
	};
	void SetQuaternion(D3DXQUATERNION qua) { m_Quaternion = qua; ArrowVector(); }
	void SetQuaternion(float x, float y, float z, float w) { m_Quaternion = D3DXQUATERNION(x, y, z, w); ArrowVector(); }
	void SetQuaternion(D3DXMATRIX matR) { D3DXQuaternionRotationMatrix(&m_Quaternion, &matR); ArrowVector(); }
	void SetPosition(D3DXMATRIX matT) { m_position.x = matT._41; m_position.y = matT._42; m_position.z = matT._43; }
	void SetPosition(NxVec3 NxPos);


	void ChangeMatrix();
	void ArrowVector();

	//방향백터를 이용해 회전 각을 계산.
	//회전 시킬 축(Default값: false(해당 방향백터의 축을 무시.)),
	//
	void VectorToYawPitchRoll(D3DXVECTOR3 vec, float*Yaw, float*Pitch, float*Roll)
	{
		D3DXVECTOR3 vec_y(0, 0, 0);
		D3DXVECTOR3 vec_p(0, 0, 0);
		vec_y = vec;
		vec_p = vec;

		vec_y.y = 0.f;
		D3DXVec3Normalize(&vec_y, &vec_y);
		float rotY_ = m_Yam;
		if (Yaw)
		{
			float dot = D3DXVec3Dot(&vec_y, &D3DXVECTOR3(0, 0, 1));
			dot = roundf(dot * 1000) / 1000.f;
			rotY_ = acos(dot);


			if (vec_y.x < 0.f)	rotY_ *= -1;
			*Yaw = rotY_;
		}

		D3DXVec3Normalize(&vec_p, &vec_p);
		float rotX_ = m_Pitch;
		if (Pitch)
		{
			//acos -> D3DXVec3Dot() 를 이용해 나온 (-1 ~ 1) 값을 라디안 각도로 변경
			float dot = D3DXVec3Dot(&vec_p, &vec_y);
			if (dot > 1.f)
			{
				dot = 0.9999f;
			}
			if (dot < -1.f)
			{
				dot = -0.9999f;
			}
			dot = roundf(dot * 1000) / 1000.f;
			rotX_ = -acos(dot);

			if (vec_p.y < 0.f) rotX_ *= -1;

			*Pitch = rotX_;
		}

		if (Roll) *Roll = m_Roll;
	}

	//vec를 이용한
	// x 와 y 축 회전 을 기본으로하며, 
	// z 값은 
	void SetQuaternionToVector(D3DXVECTOR3 vec, bool rotY, bool rotX, float* pRoll = nullptr)
	{
		D3DXVECTOR3 vec_y(0, 0, 0);
		D3DXVECTOR3 vec_p(0, 0, 0);
		vec_y = vec;
		vec_p = vec;

		if (rotY)
		{
			if (abs(vec_y.x) > 0.0001 && abs(vec_y.z) > 0.0001)
			{
				m_Yam = atan2(vec_y.x, vec_y.z);
				if (cTransform::up.y < 0)
				{
					m_Yam += D3DXToRadian(180);
				}
			}
			else
			{
				std::cout << "!";
				if (abs(vec_y.x) < 0.0001 && abs(vec_y.x) < 0.0001)
				{
					//Pass
					//위 또는 아래.
				}
				else
				{
					if (vec_y.z > 0 && abs(vec_y.x) < 0.0001)
					{
						//정면
						m_Yam = D3DXToRadian(0);
						if (cTransform::up.y < 0)
						{
							m_Yam += D3DXToRadian(180);
						}
					}
					if (vec_y.z < 0 && abs(vec_y.x) < 0.0001)
					{
						//후면
						m_Yam = D3DXToRadian(180);
						if (cTransform::up.y < 0)
						{
							m_Yam += D3DXToRadian(180);
						}
					}
					if (vec_y.x > 0 && abs(vec_y.z) < 0.0001)
					{
						//우
						m_Yam = D3DXToRadian(90);
						if (cTransform::up.y < 0)
						{
							m_Yam += D3DXToRadian(180);
						}
					}
					if (vec_y.x < 0 && abs(vec_y.z) < 0.0001)
					{
						//좌
						m_Yam = D3DXToRadian(-90);
						if (cTransform::up.y < 0)
						{
							m_Yam += D3DXToRadian(180);
						}
					}
				}
			}
		}

		D3DXVec3Normalize(&vec_p, &vec_p);
		if (rotX)
		{
			D3DXVECTOR3 rotYamVec(0, 0, 1);
			D3DXMATRIXA16 rotR;
			D3DXMatrixRotationY(&rotR, m_Yam);
			D3DXVec3TransformNormal(&rotYamVec, &rotYamVec, &rotR);

			float dot = D3DXVec3Dot(&vec_y, &rotYamVec);

			if (dot > 1.f)
			{
				dot = 0.999999f;
			}
			if (dot < -1.f)
			{
				dot = -0.999999f;
			}
			dot = roundf(dot * 100000) / 100000.f;
			float rotX_ = -acos(dot);
			if (vec_p.y < 0.f) rotX_ *= -1;

			m_Pitch = rotX_;
		}
		if (pRoll)
		{
			m_Roll = *pRoll;
		}

		D3DXQuaternionRotationYawPitchRoll(&m_Quaternion, m_Yam, m_Pitch, 0);
		ArrowVector();
	}

	//Yam	y축 회전값
	//Pitch	x축 회전값
	//Roll	z축 회전값
	//tip	Radian 값을 입력. D3DXToRadian(0~360) 
	void SetQuaternionToFloat(float Yam, float Pitch, float Roll)
	{
		m_Yam = Yam;
		m_Pitch = Pitch;
		m_Roll = Roll;
		D3DXQuaternionRotationYawPitchRoll(&m_Quaternion, m_Yam, m_Pitch, m_Roll);
		ArrowVector();
	}
	//Yam	y축 추가 회전값
	//Pitch	x축 추가 회전값
	//Roll	z축 추가 회전값
	//tip	Radian 값을 입력. D3DXToRadian(0~360) 
	void AddQuaternionToFloat(float Yam, float Pitch, float Roll)
	{
		m_Yam += Yam;
		m_Pitch += Pitch;
		m_Roll += Roll;
		D3DXQuaternionRotationYawPitchRoll(&m_Quaternion, m_Yam, m_Pitch, m_Roll);
		ArrowVector();
	}

	void Update();
	void Render();

	void RotationUp(float angle)
	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixRotationQuaternion(&matWorld, &m_Quaternion);

		D3DXQUATERNION quaR;
		D3DXQuaternionRotationAxis(&quaR, &up, angle);
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationQuaternion(&matR, &quaR);

		D3DXQuaternionRotationMatrix(&m_Quaternion, &(matWorld * matR));
		ArrowVector();
	}
	void RotationFront(float angle)
	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixRotationQuaternion(&matWorld, &m_Quaternion);

		D3DXQUATERNION quaR;
		D3DXQuaternionRotationAxis(&quaR, &front, angle);
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationQuaternion(&matR, &quaR);

		D3DXQuaternionRotationMatrix(&m_Quaternion, &(matWorld * matR));
		ArrowVector();
	}
	void RotationRight(float angle)
	{
		D3DXMATRIXA16 matWorld;
		D3DXMatrixRotationQuaternion(&matWorld, &m_Quaternion);

		D3DXQUATERNION quaR;
		D3DXQuaternionRotationAxis(&quaR, &right, angle);
		D3DXMATRIXA16 matR;
		D3DXMatrixRotationQuaternion(&matR, &quaR);

		D3DXQuaternionRotationMatrix(&m_Quaternion, &(matWorld * matR));
		ArrowVector();
	}

	D3DXMATRIXA16 GetMatrixWorld()
	{
		D3DXMATRIXA16 matS, matR, matT;
		D3DXMatrixIdentity(&matS);
		D3DXMatrixIdentity(&matR);
		D3DXMatrixIdentity(&matT);
		D3DXMatrixScaling(&matS, m_size.x, m_size.y, m_size.z);
		D3DXMatrixRotationQuaternion(&matR, &m_Quaternion);
		D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);
		return matS * matR * matT;
	}
	D3DXMATRIXA16 GetMatrixScale()
	{
		D3DXMATRIXA16 matS;
		D3DXMatrixIdentity(&matS);
		D3DXMatrixScaling(&matS, m_size.x, m_size.y, m_size.z);
		return matS;
	}
	D3DXMATRIXA16 GetMatrixRotation()
	{
		D3DXMATRIXA16 matR, matT;
		D3DXMatrixIdentity(&matR);
		D3DXMatrixRotationQuaternion(&matR, &m_Quaternion);
		return matR;
	}
	D3DXMATRIXA16 GetMatrixTranslation()
	{
		D3DXMATRIXA16 matT;
		D3DXMatrixIdentity(&matT);
		D3DXMatrixTranslation(&matT, m_position.x, m_position.y, m_position.z);
		return matT;
	}
};

