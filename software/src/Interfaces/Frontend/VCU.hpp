#pragma once

#include <QObject>
#include <filesystem>
#include <fmt/ranges.h>
#include <limits>
#include <rapidcsv.h>

namespace fake {
class VCU : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<int> currentTorqueMap MEMBER m_current_torque_map NOTIFY currentTorqueMapChanged)
    Q_PROPERTY(int profileId MEMBER m_profile_id NOTIFY profileIdChanged)
    Q_PROPERTY(int maxTorque MEMBER torque_map_max CONSTANT)
    Q_PROPERTY(int minTorque MEMBER torque_map_min CONSTANT)
    Q_PROPERTY(QList<float> currentTcTune MEMBER m_current_tc_tune NOTIFY currentTcTuneChanged)
    Q_PROPERTY(int tcTuneId MEMBER m_tc_tune_id NOTIFY tcTuneIdChanged)

  public:
    VCU(const std::string& torque_map_directory = "")
        : QObject(nullptr), m_torque_map_directory(torque_map_directory), m_profile_id(0), m_tc_tune_id(0) {

        if (!std::filesystem::exists(m_torque_map_directory) || !std::filesystem::is_directory(m_torque_map_directory)) {
            fmt::print("Torque map directory doesn't exist!");
            throw "Bricked hard";
        }

        connect(this, &VCU::profileIdChanged, &VCU::readTorqueMapCSV);
        connect(this, &VCU::tcTuneIdChanged, &VCU::readTcTuneCSV);

        readTorqueMapCSV();
        readTcTuneCSV();
    }

    Q_INVOKABLE void saveTorqueMapCSV(QList<int> torque_map) {
        auto save_path = m_torque_map_directory / fmt::format("torque_map_{}.csv", m_profile_id);
        rapidcsv::Document doc(save_path.string(), rapidcsv::LabelParams(-1, -1));
        for (qsizetype i = 0; i < torque_map.size(); i++) {
            doc.SetCell(i % 14, i / 14, torque_map.at(i));
        }
        doc.Save();
    }

    Q_INVOKABLE void saveTcTuneCSV(QList<float> tc_tune) {
        auto save_path = m_torque_map_directory / fmt::format("tc_tune_{}.csv", m_tc_tune_id);
        rapidcsv::Document doc(save_path.string(), rapidcsv::LabelParams(-1, -1));
        for (qsizetype i = 0; i < tc_tune.size(); i++) {
            doc.SetCell(i % 14, i / 14, tc_tune.at(i));
        }
        doc.Save();
    }

  public slots:
    void readTorqueMapCSV() {
        m_current_torque_map.clear();
        auto read_path = m_torque_map_directory / fmt::format("torque_map_{}.csv", m_profile_id);
        rapidcsv::Document doc(read_path.string(), rapidcsv::LabelParams(-1, -1));
        for (size_t i = 0; i < doc.GetRowCount(); i++) {
            for (const auto cell : doc.GetRow<int>(i)) {
                m_current_torque_map.push_back(cell);
            }
        }
        emit currentTorqueMapChanged();
    }

    void readTcTuneCSV() {
        m_current_tc_tune.clear();
        auto read_path = m_torque_map_directory / fmt::format("tc_tune_{}.csv", m_tc_tune_id);
        rapidcsv::Document doc(read_path.string(), rapidcsv::LabelParams(-1, -1));
        for (const auto cell : doc.GetRow<float>(0)) {
            m_current_tc_tune.push_back(cell);
        }

        emit currentTcTuneChanged();
    }

  signals:
    void currentTorqueMapChanged();
    void currentTcTuneChanged();
    void profileIdChanged();
    void tcTuneIdChanged();

  public:
    Q_INVOKABLE void sendTorqueMap(QList<int> torque_map) {
        uint8_t percent_division_count = 11; // 0-100 in increments of 10%
        uint8_t speed_division_count = 14;   // 0-130 in increments of 10kmph
        unsigned int expected_data_points_in_torque_map = percent_division_count * speed_division_count;
        if (expected_data_points_in_torque_map > 255) {
            fmt::print("Number of data points inside torque map ({}) exceeds limit of 255\n", expected_data_points_in_torque_map);
            return;
        }
        uint8_t num_of_torque_map_data_points = static_cast<uint8_t>(expected_data_points_in_torque_map);

        if (torque_map.size() != num_of_torque_map_data_points) {
            fmt::print("QML torque map size ({}) != required torque map size ({})\n", torque_map.size(), num_of_torque_map_data_points);
            return;
        }

        std::vector<uint8_t> transaction;
        transaction.reserve(num_of_torque_map_data_points);
        for (int torque : torque_map) {
            // Verify torque is within bounds
            if (torque > torque_map_max || torque < torque_map_min) {
                fmt::print("Torque map values out of bounds\n");
                return;
            }
        }
    }

    Q_INVOKABLE void sendTcTune(QList<float> tc_tune) {
        // TODO
    }

  private:
    std::filesystem::path m_torque_map_directory;
    QList<int> m_current_torque_map;
    QList<float> m_current_tc_tune;
    int m_profile_id;
    int m_tc_tune_id;

  public:
    static constexpr int8_t torque_map_offset = 128 - 25;
    static constexpr int torque_map_max = std::numeric_limits<int8_t>::max() + torque_map_offset; // Estimated is around -22Nm
    static constexpr int torque_map_min = std::numeric_limits<int8_t>::min() + torque_map_offset; // Estimated is around +230Nm
};

} // namespace fake
