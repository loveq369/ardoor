set app=ch.fhnw.comgr
set bin="z:\hudrima1 On My Mac\Documents\BFH\SLProject\trunk\_globals\GUI\Android\apk\bin"
set apk=%bin%\GLES2-release.apk
set apkunsigned=%bin%\GLES2-release-unsigned.apk
set sdk="c:\Lib\Android\adt-bundle-windows\sdk"
set jdk="C:\Program Files\Java\jdk1.7.0_07\bin"
set key="y:\KeyTool\hudrima1-release-key.keystore"
set keyuser="hudrima1"

%jdk%\jarsigner.exe -verbose -sigalg MD5withRSA -digestalg SHA1 -keystore %key% %apkunsigned% %keyuser%
del %apk%
%sdk%\tools\zipalign.exe -v 4 %apkunsigned% %apk%
%sdk%\platform-tools\adb.exe uninstall %app%
%sdk%\platform-tools\adb.exe install %apk%