#include "pch.h"
#include "WidgetProvider.h"
#include <mutex>

using namespace winrt;
using namespace Windows::Foundation;

// {838F706D-3C55-48AC-8190-45DD8CAE29B1}
static const GUID widget_provider_clsid =
{ 0x838f706d, 0x3c55, 0x48ac, { 0x81, 0x90, 0x45, 0xdd, 0x8c, 0xae, 0x29, 0xb1 } };

template <typename T>
struct SingletonClassFactory : winrt::implements<SingletonClassFactory<T>, IClassFactory>
{
    STDMETHODIMP CreateInstance(
        ::IUnknown* outer,
        GUID const& iid,
        void** result) noexcept final
    {
        *result = nullptr;

        std::unique_lock lock(mutex);

        if (outer)
        {
            return CLASS_E_NOAGGREGATION;
        }

        if (!instance)
        {
            instance = winrt::make<WidgetProvider>();
        }

        return instance.as(iid, result);
    }

    STDMETHODIMP LockServer(BOOL) noexcept final
    {
        return S_OK;
    }

private:
    T instance{ nullptr };
    std::mutex mutex;
};

int main()
{
    winrt::init_apartment();
    wil::unique_com_class_object_cookie widgetProviderFactory;
    auto factory = winrt::make<SingletonClassFactory<winrt::Microsoft::Windows::Widgets::Providers::IWidgetProvider>>();

    winrt::check_hresult(CoRegisterClassObject(
        widget_provider_clsid,
        factory.get(),
        CLSCTX_LOCAL_SERVER,
        REGCLS_MULTIPLEUSE,
        widgetProviderFactory.put()));

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}