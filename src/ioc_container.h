#include "type_id_generator.h"

#include <vector>
#include <functional>
#include <memory>

namespace IOC {

    class TTypelessServiceFactory {};

    template <typename TAbstract>
    class TTypedServiceFactory : public TTypelessServiceFactory {
    public:
        explicit TTypedServiceFactory(std::function<std::shared_ptr<TAbstract> ()> factoryImpl)
            : FactoryImpl_(factoryImpl)
        {}

        std::shared_ptr<TAbstract> GetService() {
            return FactoryImpl_();
        }

    private:
        std::function<std::shared_ptr<TAbstract> ()> FactoryImpl_;
    };

    class TServiceRegistry {
    public:
        template <typename TAbstract>
        std::shared_ptr<TAbstract> Get() {
            auto typeId = TypeIdGenerator_.Get<TAbstract>();

            const auto& abstractFactory = ServiceFactories_[typeId];
            const auto& factory = std::static_pointer_cast<TTypedServiceFactory<TAbstract>>(abstractFactory);

            return factory->GetService();
        }

        template<typename TAbstract, typename ...TDependencies>
        void RegisterFunctor(std::function<std::shared_ptr<TAbstract>(std::shared_ptr<TDependencies> ...ds)> functor) {
            TypeIdGenerator_.SetNext<TAbstract>();
            ServiceFactories_.emplace_back(std::make_shared<TTypedServiceFactory<TAbstract>>([=]{
                return functor(Get<TDependencies>()...);
            }));
        }

        template<typename TAbstract, typename ...TDependencies>
        void RegisterFunctor(std::shared_ptr<TAbstract> (*functor)(std::shared_ptr<TDependencies> ...ds)) {
            RegisterFunctor(std::function<std::shared_ptr<TAbstract>(std::shared_ptr<TDependencies> ...ds)>(functor));
        }

        template<typename TAbstract, typename TImpl, typename ...TDependencies>
        void RegisterFactory() {
            RegisterFunctor(
                std::function<std::shared_ptr<TAbstract>(std::shared_ptr<TDependencies> ...ds)>(
                    [](std::shared_ptr<TDependencies>...dependencies) -> std::shared_ptr<TAbstract> {
                        return std::make_shared<TImpl>(std::forward<std::shared_ptr<TDependencies>>(dependencies)...);
                    }));
        }

    private:
        TTypeIdGenerator TypeIdGenerator_;
        std::vector<std::shared_ptr<TTypelessServiceFactory>> ServiceFactories_;
    };

}