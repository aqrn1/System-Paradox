@echo off
echo ========================================
echo   ПОЛНАЯ ОЧИСТКА SYSTEM1PARADOX
echo ========================================
cd /d "C:\Users\aqrn1\Documents\Unreal Projects\System1Paradox"

echo 1. Удаление бинарных файлов...
if exist Binaries rd /s /q Binaries

echo 2. Удаление промежуточных файлов...
if exist Intermediate rd /s /q Intermediate

echo 3. Удаление кэша редактора...
if exist Saved rd /s /q Saved

echo 4. Удаление кэша DeriveDataCache...
if exist "C:\Users\aqrn1\AppData\Local\UnrealEngine\Common\DerivedDataCache" (
    rd /s /q "C:\Users\aqrn1\AppData\Local\UnrealEngine\Common\DerivedDataCache"
)

echo 5. Очистка кэша Visual Studio...
if exist ".vs" rd /s /q ".vs"
if exist "*.sln" del "*.sln"
if exist "*.vcxproj" del "*.vcxproj"
if exist "*.vcxproj.filters" del "*.vcxproj.filters"

echo 6. Создание структуры папок...
mkdir Binaries
mkdir Intermediate
mkdir Saved

echo ========================================
echo   ОЧИСТКА ЗАВЕРШЕНА!
echo ========================================
echo.
echo ДАЛЬНЕЙШИЕ ДЕЙСТВИЯ:
echo 1. Закройте это окно
echo 2. Откройте System1Paradox.uproject
echo 3. Нажмите "Yes" на перестройку проекта
echo ========================================
pause