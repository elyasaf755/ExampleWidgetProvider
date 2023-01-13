# ExampleWidgetProvider

## How to install
1. Open the project in VS 2022
2. Tools->Create GUID
3. Select the option "static const GUID =" and click Copy and then paste that into main.cpp, replacing the following code:<br>
`// {838F706D-3C55-48AC-8190-45DD8CAE29B1}<br>
static const GUID widget_provider_clsid =
{ 0x838f706d, 0x3c55, 0x48ac, { 0x81, 0x90, 0x45, 0xdd, 0x8c, 0xae, 0x29, 0xb1 } };`<br>
	Make sure the variable name remains `widget_provider_clsid`.
4. Copy the newly generated GUID to clipboard (currently `838F706D-3C55-48AC-8190-45DD8CAE29B1`), then right click on **Package.appxmanifest -> View Code**
5. Search for the current GUID `838F706D-3C55-48AC-8190-45DD8CAE29B1`, and replace any occurrences with the newly generated GUID.
6. Right click the **Solution -> Build Solution**
7. Right click **ExampleWidgetProviderPackage -> Deploy**
8. Open the widgets board
9. Click the "+" icon to add a widget
10. Add the counter widget

