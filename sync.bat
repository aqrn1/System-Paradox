@echo off
cd /d "C:\Users\aqrn1\Documents\Unreal Projects\System1Paradox"
echo ====================================
echo АВТОМАТИЧЕСКАЯ СИНХРОНИЗАЦИЯ PROJECT
echo ====================================
echo.

echo 1. Получаем последние изменения с GitHub...
git pull origin main

echo.
echo 2. Добавляем все новые и измененные файлы...
git add .

echo.
echo 3. Создаем коммит с текущей датой и временем...
git commit -m "Auto-sync: %date% %time%"

echo.
echo 4. Отправляем изменения на GitHub...
git push origin main

echo.
echo ====================================
echo СИНХРОНИЗАЦИЯ УСПЕШНО ЗАВЕРШЕНА!
echo ====================================
echo.
pause