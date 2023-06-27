#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

using namespace std;

namespace PaulNovack {

    class AppConfig {
    public:

        AppConfig(const std::string& filePath);
        int GO_BACK_SECONDS_FOR_SPEED;
        float MIN_SPEED_TO_RUN_STEERING;
        std::string APP_ROOT;
        float DEGREES_TURN_SIZE_XL;
        float DEGREES_TURN_SIZE_LG;
        float DEGREES_TURN_SIZE_MD;
        float DEGREES_TURN_SIZE_SM;
        float DEGREES_TURN_SIZE_XS;
        int TURN_SIZE_XL;
        int TURN_SIZE_LG;
        int TURN_SIZE_MD;
        int TURN_SIZE_SM;
        int TURN_SIZE_XS;
        int CHECK_TURN_TIME;
        int MAX_TURN_STEPS;
        int STEPS_PER_CORRECTION;
        string DB_DATABASE_NAME;
        string DB_USERNAME;
        string DB_PASSWORD;
        string DB_HOST;
    private:
    };
}
#endif  // CONFIG_HPP
