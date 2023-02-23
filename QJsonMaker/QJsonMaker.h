#pragma once

#include "qjsonmaker_global.h"
#include "qobject.h"
#include "qjsonarray.h"
#include "qdebug.h"
#include "qexception.h"
#include "qtimezone.h"
//Json相关
//给定任意模板类，将其公开属性打包成一个Json字符串，使用此方法需要所有的公开属性均为Q_PROPERTY宏声明，该类提供单例。
//序列化类Q_PROPERTY宏声明的属性 set/get函数命名规则：get/set+属性名 如getBirthday setList，大小写不限，如果是set方法需要在set方法前面加上Q_INVOKABLE 宏
//如果需要反序列化数组，请保证数组中的所有数据结构是同一个类型，否则可能会出错
//注:请尽量使用int不要使用qint32，使用double不要使用float
// 
//Json相关方法调用实例：
//
// 如果想要调用JsonMaker类来把你的类成员元素，假设是A a，其中包含元素qint32 a1,QString a2,bool a3进行封装，那么你需要使用Q_PROPERTY来
// 声明封装a1,a2,a3元素和其set/get方法（如果需要解析就需要set方法，如果需要封装就需要get方法)，set/get方法命名规则为set/get+元素名称
// 比如seta1,geta2,其中必须对大小写做出规范，set/get后面的元素名称必须与大小写保持一致，否则可能
// 无法正常使用该JsonMaker
// 
// 调用方法如下:
// 1.封装字符串
// A a; 
// QString result = JsonMaker::Singleton().JsonSerialization<Tester1>(tester);
// 2.解析字符串
// QSharedPointer<A> a;
// JsonMaker::Singleton().JsonDeserialization<Tester1>(a, Json);
// 调用完毕后a中的对应数据都会被Json字符串中的数据覆盖
// 详情可以参考Tester1类
//
template<class T1>
QString QJSONMAKER_EXPORT JsonSerialization(QSharedPointer<T1>T_Class_1);

template<class T>
void QJSONMAKER_EXPORT JsonDeserialization(QSharedPointer<T> T_Class, const QString& jsonString);

