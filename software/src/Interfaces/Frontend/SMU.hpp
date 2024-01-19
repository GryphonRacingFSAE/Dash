#pragma once

#include <Frontend/FakeInterface.hpp>
#include <QObject>
#include <cstdlib>

namespace fake {

class SMU : public QObject, public FakeInterface {
    Q_OBJECT
    Q_PROPERTY(QList<int> temperatures MEMBER m_temperatures NOTIFY temperaturesChanged)
  public:
    SMU(const std::string& /* dbc_file_path */ = "") : QObject(nullptr), m_temperatures(5 * 56, -40) {
        this->FakeInterface::startReceiving();
    }
    std::chrono::milliseconds delay{1000};

  signals:
    void newThermistorTemp(int segment, int id, int temp);
    void temperaturesChanged();

  private:
    QList<int> m_temperatures;

  private:
    void generateValues() {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 56; j++) {
                int temp = rand() % 121 - 40;
                m_temperatures[i * 56 + j] = temp;
                emit newThermistorTemp(i, j, temp);
            }
        }
        // emit temperaturesChanged();
    }
};

} // namespace fake
