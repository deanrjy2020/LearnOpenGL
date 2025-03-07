编译, 用自带的cmake编译, 中间文件在build里面, output在bin里面
cmake -S . -B build -G "Visual Studio 17 2022" && cmake --build build --config Debug --parallel
编译单个:
cmake --build build --config Debug --target 1.getting_started__7.6.camera_exercise2 --parallel

单步配置
安装2个
C/C++（Microsoft，扩展 ID：ms-vscode.cpptools）
CMake Tools（Microsoft，扩展 ID：ms-vscode.cmake-tools）
    在 VS Code：
    1. 按 Ctrl+Shift+P
    2. 运行 CMake: Select a Kit
    3. Scan for Kits, 选择类似以下的项：
    - Visual Studio Community 2019 Release - amd64
    - 或 Visual Studio Build Tools 2019 Release - amd64
reload window

todo, 还没仔细看, 以后回来再看
    7.6, 如何手动计算MVP matrix, 等
    412, depth value viewer, 如何得到?
    5, adv lingtning, 以后用到了再看
