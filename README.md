# Virtual Engine ECU — FreeRTOS Windows Demo

Donanım gerektirmeden Windows üzerinde çalışan, mülakatta anlatılabilecek küçük bir embedded yazılım demosu. Gerçek FreeRTOS scheduler/queue API'lerini; simüle edilmiş ADC, PWM, CAN ve watchdog sürücüleriyle kullanır.

> Bu bir PC simülasyonudur. Windows portu gerçek zamanlama garantisi vermez ve gerçek MCU register'larına erişmez.

## Demo ne gösteriyor?

- `ControlTask`: Her 5 saniyede (`vTaskDelayUntil`) ADC sıcaklığını okur, PWM fan hızını hesaplar ve watchdog'u besler.
- `CanTask`: Tek elemanlı FreeRTOS queue'dan son telemetriyi bekler ve CAN frame'ini konsola basar.
- `F` tuşu: Watchdog beslemesini keser. Yaklaşık 10 saniye içinde timeout ve simüle reset görülür; ardından sistem normal çalışmaya döner.
- Program başlangıç ekranında bekler; `S` tuşuna basılınca simülasyon başlar.
- Katmanlar: Application/Tasks → Driver interface → HardwareSim.

```text
ControlTask --> ADC Driver --> HardwareSim
     |-----> PWM Driver --> HardwareSim
     |-----> Queue -----> CanTask --> CAN Driver --> HardwareSim
     `-----> Watchdog Driver
```

## Klasör yapısı

```text
VirtualEngineECU/
├── include/
│   ├── Tasks.hpp
│   ├── Drivers.hpp
│   ├── HardwareSim.hpp
│   └── FreeRTOSConfig.h
├── src/
│   ├── main.cpp
│   ├── Tasks.cpp
│   ├── Drivers.cpp
│   └── HardwareSim.cpp
├── samples/
│   └── expected-output.txt    # Örnek konsol çıktısı
├── CMakeLists.txt
├── .gitignore
├── .gitmodules
└── third_party/
    └── FreeRTOS-Kernel/       # git submodule ile gelir
```

## 1 — Visual Studio kurulumu

1. [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) kurucusunu indir ve aç.
2. **Desktop development with C++** iş yükünü seç.
3. Sağdaki bileşenlerde **MSVC**, **Windows 10/11 SDK**, **C++ CMake tools for Windows** seçili olsun.
4. **Install** düğmesine bas ve kurulum bitince Visual Studio'yu aç.
5. Git eksikse [Git for Windows](https://git-scm.com/download/win) kur. Varsayılan seçenekler yeterlidir.

## 2 — Projeyi indirme ve FreeRTOS'u ekleme

GitHub'a koyduktan sonra yeni bir bilgisayarda en kolay yöntem:

```powershell
git clone --recurse-submodules https://github.com/KULLANICI_ADIN/VirtualEngineECU.git
cd VirtualEngineECU
```

Bu klasörü henüz GitHub'a göndermeden, mevcut proje içinde FreeRTOS'u ilk kez eklemek için:

```powershell
git submodule add https://github.com/FreeRTOS/FreeRTOS-Kernel.git third_party/FreeRTOS-Kernel
git -C third_party/FreeRTOS-Kernel checkout V11.2.0
```

`.gitmodules` dosyası zaten mevcutsa ilk komut “already exists” diyebilir. Bu durumda:

```powershell
git submodule update --init --recursive
git -C third_party/FreeRTOS-Kernel checkout V11.2.0
```

Sürümü sabitlemek önemlidir: ileride kernel değişse bile demo aynı kaynakla derlenir.

## 3 — Visual Studio'da açma

1. Visual Studio ana ekranında **Open a local folder** seç.
2. Bu projenin kök klasörünü (`VirtualEngineECU`) seç.
3. Visual Studio alt durum çubuğunda CMake yapılandırmasının bitmesini bekle.
4. Üst çubukta hedef olarak **VirtualEngineECU.exe** ve platform olarak **x64-Debug** seç.

Kırmızı “FreeRTOS-Kernel is missing” mesajı görürsen Visual Studio terminalini açıp şunu çalıştır:

```powershell
git submodule update --init --recursive
```

Sonra **Project → Delete Cache and Reconfigure** seç.

## 4 — Build ve çalıştırma

1. **Build → Build All** (`Ctrl+Shift+B`).
2. **Debug → Start Without Debugging** (`Ctrl+F5`).
3. Başlangıç ekranındaki kısa açıklamayı ve tuş listesini oku.
4. Simülasyonu başlatmak için `S` tuşuna bas.
5. Konsolda her 5 saniyede sıcaklık, fan yüzdesi ve `CAN ID=0x180` görünecek.
6. Konsol penceresi seçiliyken `F` tuşuna bir kez bas.
7. Yaklaşık 10 saniye içinde `WATCHDOG TIMEOUT` ve simüle ECU reset mesajı görünür.
8. Çıkmak için `Ctrl+C` kullan.

Terminalden alternatif build:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Debug
.\build\Debug\VirtualEngineECU.exe
```

## 5 — GitHub'a yükleme (bebek adımları)

1. [github.com/new](https://github.com/new) adresinde `VirtualEngineECU` adlı **boş** repository oluştur. README veya `.gitignore` ekleme; ikisi de burada var.
2. Proje klasöründe terminal aç.
3. Aşağıdaki komutları sırayla çalıştır (`KULLANICI_ADIN` kısmını değiştir):

```powershell
git init
git add .
git commit -m "Initial Virtual Engine ECU FreeRTOS demo"
git branch -M main
git remote add origin https://github.com/KULLANICI_ADIN/VirtualEngineECU.git
git push -u origin main
```

Git ad/eposta isterse bir kez ayarla:

```powershell
git config --global user.name "Ad Soyad"
git config --global user.email "eposta@example.com"
```

GitHub parola kabul etmezse tarayıcıyla oturum açan **Git Credential Manager** penceresini tamamla.

## Mülakatta 30 saniyelik anlatım

“Donanım olmadan mimariyi göstermek için Windows üzerinde FreeRTOS'un MSVC portunu kullandım. Control task periyodik ADC örnekliyor, fan PWM'ini hesaplıyor ve tek elemanlı queue üzerinden CAN task'a en güncel telemetriyi iletiyor. Driver arayüzlerini HardwareSim katmanından ayırdım; MCU'ya taşırken application ve task mantığını koruyup yalnızca alt katmanı değiştirebilirim. F tuşuyla watchdog beslemesini keserek timeout ve safe-state davranışını gösterebiliyorum.”

## Notlar

- FreeRTOS Windows portu çok çekirdekli bir Windows makine bekler.
- `xQueueOverwrite`, uzunluk 1 olan queue ile “en güncel değer” davranışı sağlar.
- Bu demo eğitim/mimari gösterimi içindir; safety-critical ürün kodu değildir.
- FreeRTOS-Kernel kendi MIT lisansı altında bir submodule olarak tutulur.
