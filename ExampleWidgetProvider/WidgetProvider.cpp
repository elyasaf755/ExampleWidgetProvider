#include "pch.h"
#include "WidgetProvider.h"

namespace winrt
{
    using namespace Microsoft::Windows::Widgets::Providers;
}

std::unordered_map<winrt::hstring, CompactWidgetInfo> WidgetProvider::RunningWidgets{};

WidgetProvider::WidgetProvider()
{
    auto runningWidgets = winrt::WidgetManager::GetDefault().GetWidgetInfos();
    for (auto widgetInfo : runningWidgets)
    {
        auto widgetContext = widgetInfo.WidgetContext();
        auto widgetId = widgetContext.Id();
        auto widgetName = widgetContext.DefinitionId();
        auto customState = widgetInfo.CustomState();
        if (RunningWidgets.find(widgetId) == RunningWidgets.end())
        {
            CompactWidgetInfo runningWidgetInfo{ widgetId, widgetName };
            try
            {
                // If we had any save state (in this case we might have some state saved for Counting widget)
                // convert string to required type if needed.
                int count = std::stoi(winrt::to_string(customState));
                runningWidgetInfo.customState = count;
            }
            catch (...)
            {

            }
            RunningWidgets[widgetId] = runningWidgetInfo;
        }
    }
}

void WidgetProvider::CreateWidget(winrt::WidgetContext widgetContext)
{
    auto widgetId = widgetContext.Id();
    auto widgetName = widgetContext.DefinitionId();
    CompactWidgetInfo runningWidgetInfo{ widgetId, widgetName };
    RunningWidgets[widgetId] = runningWidgetInfo;

    // Update the widget
    UpdateWidget(runningWidgetInfo);
}

void WidgetProvider::DeleteWidget(winrt::hstring const& widgetId, winrt::hstring const& customState)
{
    RunningWidgets.erase(widgetId);
}

void WidgetProvider::OnActionInvoked(winrt::WidgetActionInvokedArgs actionInvokedArgs)
{
    auto verb = actionInvokedArgs.Verb();
    if (verb == L"inc")
    {
        auto widgetId = actionInvokedArgs.WidgetContext().Id();
        // If you need to use some data that was passed in after
        // Action was invoked, you can get it from the args:
        auto data = actionInvokedArgs.Data();
        if (const auto iter = RunningWidgets.find(widgetId); iter != RunningWidgets.end())
        {
            auto& localWidgetInfo = iter->second;
            // Increment the count
            localWidgetInfo.customState++;
            UpdateWidget(localWidgetInfo);
        }
    }
}

void WidgetProvider::OnWidgetContextChanged(winrt::WidgetContextChangedArgs contextChangedArgs)
{
    auto widgetContext = contextChangedArgs.WidgetContext();
    auto widgetId = widgetContext.Id();
    auto widgetSize = widgetContext.Size();
    if (const auto iter = RunningWidgets.find(widgetId); iter != RunningWidgets.end())
    {
        auto localWidgetInfo = iter->second;

        UpdateWidget(localWidgetInfo);
    }
}

void WidgetProvider::Activate(winrt::Microsoft::Windows::Widgets::Providers::WidgetContext widgetContext)
{
    auto widgetId = widgetContext.Id();

    if (const auto iter = RunningWidgets.find(widgetId); iter != RunningWidgets.end())
    {
        auto& localWidgetInfo = iter->second;
        localWidgetInfo.isActive = true;

        UpdateWidget(localWidgetInfo);
    }
}

void WidgetProvider::Deactivate(winrt::hstring widgetId)
{
    if (const auto iter = RunningWidgets.find(widgetId); iter != RunningWidgets.end())
    {
        auto& localWidgetInfo = iter->second;
        localWidgetInfo.isActive = false;
    }
}

void WidgetProvider::UpdateWidget(CompactWidgetInfo const& localWidgetInfo)
{
    winrt::WidgetUpdateRequestOptions updateOptions{ localWidgetInfo.widgetId };

    winrt::hstring templateJson;
    if (localWidgetInfo.widgetName == L"Counting_Widget")
    {
        templateJson = winrt::to_hstring(countWidgetTemplate);
    }

    winrt::hstring dataJson;
    if (localWidgetInfo.widgetName == L"Counting_Widget")
    {
        dataJson = L"{ \"count\": " + winrt::to_hstring(localWidgetInfo.customState) + L" }";
    }

    updateOptions.Template(templateJson);
    updateOptions.Data(dataJson);
    // You can store some custom state in the widget service that you will be able to query at any time.
    updateOptions.CustomState(winrt::to_hstring(localWidgetInfo.customState));
    winrt::WidgetManager::GetDefault().UpdateWidget(updateOptions);
}