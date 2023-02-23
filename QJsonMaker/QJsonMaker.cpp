#include "QJsonMaker.h"

template<class T1>
QString JsonSerialization(QSharedPointer<T1>T_Class_1) {
	auto T_Class = dynamic_cast<QObject*>(T_Class_1.data());
	QJsonObject jsonObject;
	//ͨ��Ԫ�������Ա
	const QMetaObject* metaObject = T_Class->metaObject();
	for (int i = 0; i < metaObject->propertyCount(); ++i) {
		QMetaProperty property = metaObject->property(i);

		if (!property.isReadable()) {
			continue;
		}
		//�����֪����ʲô����ʱ��Ҫ�����ε�

		if (QString(property.name()) == "objectName") {
			continue;
		}

		//�����QList

		if (QString(property.typeName()).contains("QList")) {
			//�������Ҫ���ݳ������ͽ���һ�·���
			QJsonArray jsonListArray;
			//����һ��ģ�������ͣ����һ��jsonObject
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
		//�������QList
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
	// ʹ��QMetaObject��invokeMethod()����������ģ����T��setter����
	const QMetaObject* metaObject = qobject->metaObject();

	for (int i = 0; i < metaObject->propertyCount(); ++i) {
		QMetaProperty property = metaObject->property(i);
		if (property.isReadable() && property.isWritable()) {
			QString propertyName = property.name();
			QString str_functinoname = QString("set" + propertyName);
			//Ϊ��ת����const char*���ͱ����һ���м䲽��
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
					//�������������Ҫ����������н���
					if (!value.isArray()) {
						break;
					}
					QJsonArray arr = value.toArray();
					//����ȷ����������
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
		//�ж�����
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
			//��ΪΪ����
			if (arr[0].toDouble() == arr[0].toInt()) {
				qDebug() << arr[0].toDouble() << arr[0].toInt();
				QList<qint32> list_result;
				QList<int> list_result_2;
				QJsonValue value;

				for each (QJsonValue temp_value in arr)
				{
					//int �� qint32����Ҫ���ԣ����뾡������ʹ��qint32����δ���ռ�����������ڴ棬�������ܿ���ɾ��
					list_result.append(temp_value.toInt());
					list_result_2.append(temp_value.toInt());
				}
				if (!QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<qint32>, list_result))) {
					QMetaObject::invokeMethod(qobject, func_name, Q_ARG(QList<int>, list_result_2));
				}
			}
			//��Ϊ˫����
			else {
				QList<double> list_result;
				QList<float> list_result_2;
				QJsonValue value;

				for each (QJsonValue temp_value in arr)
				{
					list_result.append(temp_value.toDouble());
				}
				//double��float���᳢�ԣ��뾡��ʹ��double
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