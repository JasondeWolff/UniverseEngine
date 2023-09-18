for /r %%i in (*.vert) do C:\VulkanSDK\1.3.261.1\Bin\glslc.exe %%i -o %%i.spv
for /r %%i in (*.frag) do C:\VulkanSDK\1.3.261.1\Bin\glslc.exe %%i -o %%i.spv