#pragma once

#include "qjsonmaker_global.h"
#include "qobject.h"
#include "qjsonarray.h"
#include "qdebug.h"
#include "qexception.h"
#include "qtimezone.h"
//Json���
//��������ģ���࣬���乫�����Դ����һ��Json�ַ�����ʹ�ô˷�����Ҫ���еĹ������Ծ�ΪQ_PROPERTY�������������ṩ������
//���л���Q_PROPERTY������������ set/get������������get/set+������ ��getBirthday setList����Сд���ޣ������set������Ҫ��set����ǰ�����Q_INVOKABLE ��
//�����Ҫ�����л����飬�뱣֤�����е��������ݽṹ��ͬһ�����ͣ�������ܻ����
//ע:�뾡��ʹ��int��Ҫʹ��qint32��ʹ��double��Ҫʹ��float
// 
//Json��ط�������ʵ����
//
// �����Ҫ����JsonMaker������������ԱԪ�أ�������A a�����а���Ԫ��qint32 a1,QString a2,bool a3���з�װ����ô����Ҫʹ��Q_PROPERTY��
// ������װa1,a2,a3Ԫ�غ���set/get�����������Ҫ��������Ҫset�����������Ҫ��װ����Ҫget����)��set/get������������Ϊset/get+Ԫ������
// ����seta1,geta2,���б���Դ�Сд�����淶��set/get�����Ԫ�����Ʊ������Сд����һ�£��������
// �޷�����ʹ�ø�JsonMaker
// 
// ���÷�������:
// 1.��װ�ַ���
// A a; 
// QString result = JsonMaker::Singleton().JsonSerialization<Tester1>(tester);
// 2.�����ַ���
// QSharedPointer<A> a;
// JsonMaker::Singleton().JsonDeserialization<Tester1>(a, Json);
// ������Ϻ�a�еĶ�Ӧ���ݶ��ᱻJson�ַ����е����ݸ���
// ������Բο�Tester1��
//
template<class T1>
QString QJSONMAKER_EXPORT JsonSerialization(QSharedPointer<T1>T_Class_1);

template<class T>
void QJSONMAKER_EXPORT JsonDeserialization(QSharedPointer<T> T_Class, const QString& jsonString);

