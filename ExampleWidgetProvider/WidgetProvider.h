struct CompactWidgetInfo
{
	winrt::hstring widgetId;
	winrt::hstring widgetName;
	int customState = 0;
	bool isActive = false;
};

struct WidgetProvider : winrt::implements<WidgetProvider, winrt::Microsoft::Windows::Widgets::Providers::IWidgetProvider>
{
	WidgetProvider();

	/* IWidgetProvider required functions that need to be implemented */
	void CreateWidget(winrt::Microsoft::Windows::Widgets::Providers::WidgetContext WidgetContext);
	void DeleteWidget(winrt::hstring const& widgetId, winrt::hstring const& customState);
	void OnActionInvoked(winrt::Microsoft::Windows::Widgets::Providers::WidgetActionInvokedArgs actionInvokedArgs);
	void OnWidgetContextChanged(winrt::Microsoft::Windows::Widgets::Providers::WidgetContextChangedArgs contextChangedArgs);
	void Activate(winrt::Microsoft::Windows::Widgets::Providers::WidgetContext widgetContext);
	void Deactivate(winrt::hstring widgetId);
	/* IWidgetProvider required functions that need to be implemented */

private:

	static std::unordered_map<winrt::hstring, CompactWidgetInfo> RunningWidgets;
	void UpdateWidget(CompactWidgetInfo const& localWidgetInfo);
};

const std::string countWidgetTemplate = R"(
{
    "type": "AdaptiveCard",
    "$schema": "http://adaptivecards.io/schemas/adaptive-card.json",
    "version": "1.6",
    "backgroundImage": {
        "url": "https://images.unsplash.com/photo-1579546929518-9e396f3cc809?ixlib=rb-4.0.3&ixid=MnwxMjA3fDB8MHxleHBsb3JlLWZlZWR8MXx8fGVufDB8fHx8&w=1000&q=80",
        "fillMode": "Cover"
    },
    "body": [
        {
            "type": "TextBlock",
            "text": "Counting Widget Demo",
            "wrap": true,
            "horizontalAlignment": "center",
            "style": "heading"
        },
        {
            "type": "TextBlock",
            "text": "Button was clicked ${count} times",
            "wrap": true
        },
        {
            "type": "TextBlock",
            "text": "Render if Small",
            "wrap": true,
            "$when": "${$host.widgetSize==\"small\"}"
        },
        {
            "type": "TextBlock",
            "text": "Render if Medium",
            "wrap": true,
            "$when": "${$host.widgetSize==\"medium\"}"
        },
        {
            "type": "TextBlock",
            "text": "Render if Large",
            "wrap": true,
            "$when": "${$host.widgetSize==\"large\"}"
        },
        {
            "type": "ActionSet",
            "actions": [
                {
                    "type": "Action.Execute",
                    "title": "Increment",
                    "style": "positive",
                    "verb": "inc"
                }
            ],
            "horizontalAlignment": "center"
        }
    ]
})";

