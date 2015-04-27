#ifndef SENSORS_H
#define SENSORS_H

#include <QObject>
#include <QTextStream>
#include <QDebug>

#include "hwmon.h"

class Hwmon;

class Sensor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint index READ index NOTIFY indexChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Hwmon * parent READ parent CONSTANT)

public:

    explicit Sensor(Hwmon *parent, uint index);

    QString name() const { return m_name; }
    void setName(const QString &name) { if (name != m_name) { m_name = name; emit nameChanged(); } }
    Hwmon * parent() const { return m_parent; }
    uint index() const { return m_index; }


public slots:

    virtual void update() = 0;


signals:

    void nameChanged();
    void indexChanged();


protected:

    QString m_name;
    Hwmon *m_parent;
    uint m_index;
};


class Temp : public Sensor
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label NOTIFY labelChanged)
    Q_PROPERTY(int value READ value NOTIFY valueChanged)

public:

    explicit Temp(Hwmon *parent, uint index);

    QString label() const { return m_label; }
    int value() const { return m_value; }


public slots:

    void update();


signals:

    void labelChanged();
    void valueChanged();


protected:

    QString m_label;
    int m_value;
    QTextStream m_valueStream;
};


class Fan : public Sensor
{
    Q_OBJECT
    Q_PROPERTY(int rpm READ rpm NOTIFY rpmChanged)

public:

    explicit Fan(Hwmon *parent, uint index);

    int rpm() const { return m_rpm; }

    virtual int pwm() const { return 0; }
    virtual void setPwm(int) { qDebug() << "setPwm(int) is not implemented in standard Fan"; }


signals:

    void rpmChanged();


public slots:

    void update();


protected:

    int m_rpm;
    QTextStream m_rpmStream;
};


class PwmFan : public Fan
{
    Q_OBJECT
    Q_PROPERTY(int pwm READ pwm WRITE setPwm NOTIFY pwmChanged)
    Q_PROPERTY(Temp * temp READ temp WRITE setTemp NOTIFY tempChanged)
    Q_PROPERTY(bool hasTemp READ hasTemp WRITE setHasTemp NOTIFY hasTempChanged)
    Q_PROPERTY(int minTemp READ minTemp WRITE setMinTemp NOTIFY minTempChanged)
    Q_PROPERTY(int maxTemp READ maxTemp WRITE setMaxTemp NOTIFY maxTempChanged)
    Q_PROPERTY(int minPwm READ minPwm WRITE setMinPwm NOTIFY minPwmChanged)
    Q_PROPERTY(int maxPwm READ maxPwm WRITE setMaxPwm NOTIFY maxPwmChanged)
    Q_PROPERTY(int minStart READ minStart WRITE setMinStart NOTIFY minStartChanged)
    Q_PROPERTY(int minStop READ minStop WRITE setMinStop NOTIFY minStopChanged)

public:

    explicit PwmFan(Hwmon *parent, uint index);

    int pwm() const { return m_pwm; }
    void setPwm(int pwm) { if (pwm != m_pwm) { m_pwm = pwm; emit pwmChanged(); writePwm(); } }
    Temp * temp() const { return m_temp; }
    bool hasTemp() const { return m_hasTemp; }
    int minTemp() const { return m_minTemp; }
    int maxTemp() const { return m_maxTemp; }
    int minPwm() const { return m_minPwm; }
    int maxPwm() const { return m_maxPwm; }
    int minStart() const { return m_minStart; }
    int minStop() const { return m_minStop; }
    void setTemp(Temp *temp) { setHasTemp(temp != nullptr); if (temp != m_temp) { m_temp = temp; emit tempChanged(); } }
    void setHasTemp(bool hasTemp) { if (hasTemp != m_hasTemp) { m_hasTemp = hasTemp; emit hasTempChanged(); } }
    void setMinTemp(int minTemp) { if (minTemp != m_minTemp) { m_minTemp = minTemp; emit minTempChanged(); } }
    void setMaxTemp(int maxTemp) { if (maxTemp != m_maxTemp) { m_maxTemp = maxTemp; emit maxTempChanged(); } }
    void setMinPwm(int minPwm) { if (minPwm != m_minPwm) { m_minPwm = minPwm; emit minPwmChanged(); } }
    void setMaxPwm(int maxPwm) { if (maxPwm != m_maxPwm) { m_maxPwm = maxPwm; emit maxPwmChanged(); } }
    void setMinStart(int minStart) { if (minStart != m_minStart) { m_minStart = minStart; emit minStartChanged(); } }
    void setMinStop(int minStop) { if (minStop != m_minStop) { m_minStop = minStop; emit minStopChanged(); } }
    void reset();


signals:

    void pwmChanged();
    void tempChanged();
    void hasTempChanged();
    void minTempChanged();
    void maxTempChanged();
    void minPwmChanged();
    void maxPwmChanged();
    void minStartChanged();
    void minStopChanged();


protected slots:

    void update();
    void writePwm();


protected:

    int m_pwm;
    QTextStream m_pwmStream;
    Temp *m_temp;
    bool m_hasTemp;
    int m_minTemp;
    int m_maxTemp;
    int m_minPwm;
    int m_maxPwm;
    int m_minStart;
    int m_minStop;
};

#endif // SENSORS_H
