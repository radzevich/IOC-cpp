#pragma once

namespace IOC {

    class TTypeIdGenerator {
    public:
        TTypeIdGenerator() {
            NextTypeId_ = 0;
        }

        template<typename T>
        static int Get() {
            return GetOrSet<T>();
        }

        template<typename T>
        static void SetNext() {
            GetOrSet<T>(NextTypeId_++);
        }

    private:
        template<typename T>
        static int GetOrSet(int value = -1)
        {
            static int capturedTypeId = 0;
            if (value != -1) {
                capturedTypeId = value;
            }

            return capturedTypeId;
        }

    private:
        static int NextTypeId_;
    };

}
