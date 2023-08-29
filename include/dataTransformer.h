#include <cstdlib>
#include <fstream>
#include "TarObject.h"

class dataTransformer{
    private:
        TarObeject _tarObject;
    public:
        dataTransformer();

        void setTarObject(TarObeject tarObject);
        
        void sendTarObject();
        /**
         * @brief send the tarObject to the svm
        */
        bool isResult();
        /**
         * @brief if the model get the result, return the result
        */
        TarObeject getResult();
        /**
         * @brief get the result from the model
        */
};

