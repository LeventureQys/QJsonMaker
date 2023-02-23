#include "QJsonMaker.h"

template<class T1>
QString JsonSerialization(QSharedPointer<T1>T_Class_1) {
	auto T_Class = dynamic_cast<QObject*>(T_Class_1.data());
	QJsonObject jsonObject;
	//通过元对象定义成员
	const QMetaObject* metaObject = T_Class->metaObject();
	for (int i = 0; i < metaObject->propertyCount(); ++i) {
		QMetaProperty property = metaObject->property(i);

		if (!property.isReadable()) {
			continue;
		}
		//这个不知道是什么，暂时需要先屏蔽掉

		if (QString(property.name()) == "objectName") {
			continue;
		}

		//如果是QList

		if (QString(property.typeName()).contains("QList")) {
			//这里可能要根据常见类型进行一下分类
			QJsonArray jsonListArray;
			//输入一个模板类类型，输出一个jsonObject
			if (QString(property.typeName()) == "QList<QString>") {
				QList<QString> str_message = property.read(T_Class).value<QList<QString>>();
				jsonListArray = QListToJsonArray(str_message);
			}
			else if (QString(property.typeName()) == "QList<qint32>" || QString(property.typeName()) == "QList<int>") {
				QList<qint32> str_message = property.read(T_Class).value<QList<qint32>>();
				jsonListArray = QListToJsonArray(str_message);
			}
			else if (QString(property.typeName()) == "QList<qint64>") {
				QList<qint64> str_message = property.read(T_Class).value<QList<qint64>>();
				jsonListArray = QListToJsonArray(str_message);
			}
			else if (QString(property.typeName()) == "QList<int>") {
				QList<int> str_message = property.read(T_Class).value<QList<int>>();
				jsonListArray = QListToJsonArray(str_message);
			}
			else if (QString(property.typeName()) == "QList<bool>") {
				QList<bool> str_message = property.read(T_Class).value<QList<bool>>();
				jsonListArray = QListToJsonArray(str_message);
			}
			else if (QString(property.typeName()) == "QList<double>") {
				QList<double> str_message = property.read(T_Class).value<QList<double>>();
				jsonListArray = QListToJsonArray(str_message);
			}
			else if (QString(property.typeName()) == "QList<Float>") {
				QList<float> str_message = property.read(T_Class).value<QList<float>>();
				jsonListArray = QListToJsonArray(str_message);
			}
			else if (QString(property.typeName()) == "QList<QByteArray>") {
				QList<QByteArray> str_message = property.read(T_Class).value<QList<QByteArray>>();
				jsonListArray = QListToJsonArray(str_message);
			}

			jsonObject.insert(property.name(), QJsonValue(jsonListArray));
		}
		//如果不是QList
		else {
			QVariant result = property.read(T_Class);
			jsonObject[property.name()] = QJsonValue::fromVariant(property.read(T_Class));

		}
		qDebug() << property.name();
	}
	QJsonDocument doc(jsonObject);
	return doc.toJson(QJsonDocument::Compact);
}

template<class T>
void JsonDeserialization(QSharedPointer<T> T_Class, const QString& jsonString)
{
	auto qobject = dynamic_cast<QObject*>(T_Class.data());

	QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
	QJsonObject jsonObject = doc.object();
	// 使用QMetaObject的invokeMethod()函数来调用模板类T的setter函数
	const QMetaObject* metaObject = qobject->metaObject();

	for (int i = 0; i < metaObject->propertyCount(); ++i) {
		QMetaProperty property = metaObject->property(i);
		if (property.isReadable() && property.isWritable()) {
			QString propertyName = property.name();
			QString str_functinoname = QString("set" + propertyName);
			//为了转换成const char*类型必须的一个中间步骤
			QByteArray temp_qba_functinoname = str_functinoname.toLocal8Bit();
			const char* func_name = temp_qba_functinoname.data();

			if (jsonObject.contains(propertyName)) {
				QJsonValue value = jsonObject[propertyName];
				JsonMaker temp;
				qDebug() << value;
				switch (value.type()) {
				case QJsonValue::Type::Bool:
					QMetaObject::invokeMethod(qobject, func_name, Q_ARG(bool, value.toBool()));
					break;
				case QJsonValue::Type::Double:
					if (!QMetaObject::invokeMethod(qobject, func_name, Q_ARG(int, value.toDouble())))
						QMetaObject::invokeMethod(qobject, func_name, Q_ARG(double, value.toDouble()));
					break;
				case QJsonValue::Type::String:
					qDebug() << QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QString, value.toString()));
					break;
				case QJsonValue::Type::Array: {
					//如果是数组则需要根据情况进行解析
					if (!value.isArray()) {
						break;
					}
					QJsonArray arr = value.toArray();
					//下面确定数组类型
					this->JsonArrayDeserialization(qobject, func_name, arr);
				}
											break;
				case QJsonValue::Type::Object:
					QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QJsonValue, value));
					break;
				default:
					break;
				}
			}
		}
	}
}

template<class T>
QJsonArray QListToJsonArray(QList<T> list) {
	QJsonArray jsonArray;
	for each (T temp_T in list)
	{
		jsonArray.append(QJsonValue::fromVariant(temp_T));
	}

	return jsonArray;
}

void JsonArrayDeserialization(QObject* qobject, const char* func_name, QJsonArray arr) {
	try {
		//判断类型
	   //QString
		if (arr[0].type() == QJsonValue::String) {
			QList<QString> list_result;
			QJsonValue value;

			for each (QJsonValue temp_value in arr)
			{
				list_result.append(temp_value.toString());
			}
			QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<QString>, list_result));

		}
		else if (arr[0].isDouble()) {
			//若为为整形
			if (arr[0].toDouble() == arr[0].toInt()) {
				qDebug() << arr[0].toDouble() << arr[0].toInt();
				QList<qint32> list_result;
				QList<int> list_result_2;
				QJsonValue value;

				for each (QJsonValue temp_value in arr)
				{
					//int 和 qint32都需要尝试，但请尽量尝试使用qint32，这段代码占用了两倍的内存，将来可能考虑删除
					list_result.append(temp_value.toInt());
					list_result_2.append(temp_value.toInt());
				}
				if (!QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<qint32>, list_result))) {
					QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<int>, list_result_2));
				}
			}
			//若为双精度
			else {
				QList<double> list_result;
				QList<float> list_result_2;
				QJsonValue value;

				for each (QJsonValue temp_value in arr)
				{
					list_result.append(temp_value.toDouble());
				}
				//double和float都会尝试，请尽量使用double
				if (!QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<double>, list_result))) {
					QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<float>, list_result_2));
				}

			}
		}if (arr[0].type() == QJsonValue::Bool) {
			QList<bool> list_result;
			QJsonValue value;

			for each (QJsonValue temp_value in arr)
			{
				list_result.append(temp_value.toBool());
			}
			QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<bool>, list_result));
		}
	}
	catch (const QException& e) {
		qDebug() << "JsonArrayDeserialization" << "JsonArrayDeserialization" << e.what();
	}
}