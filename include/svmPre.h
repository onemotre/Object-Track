#include <cstdlib>
#include <fstream>
#include "TarObject.h"

class svmPre{
    private:
        TarObeject _tarObject;
    public:
        svmPre();

        void setTarObject(TarObeject tarObject);
        
        void sendTarObject();
        /**
         * @brief send the tarObject to the svm
        */
        bool isResult();
        /**
         * @brief if the svm get the result, return the result
        */
        TarObeject getResult();
        /**
         * @brief get the result from the svm
        */
};

