# Sosyal Ağ Analiz Sistemi

## Genel Bakış
Bu proje, kullanıcıların sosyal ağ verilerini yönetmesini, analiz etmesini ve birbirleriyle ilişki kurmasını sağlayan interaktif bir sistemdir. Projede;
- **Kullanıcı Yönetimi:** Yeni kullanıcı ekleme, listeleme gibi temel işlemler,
- **Arkadaşlık Yönetimi:** Kullanıcılar arasında arkadaşlık ilişkilerinin tanımlanması,
- **Analiz Modülleri:** DFS (derinlik öncelikli arama) ile arkadaşlık yollarının kontrolü, topluluk tespiti ve etki alanı hesaplama,
- **Red–Black Tree Uygulaması:** Hızlı veri erişimi için kırmızı-siyah ağaç yapısının kullanımı,
- **Düzenli Veri Kaydı:** Veriler, `veriseti.txt` dosyasında önce kullanıcı bilgileri, ardından arkadaşlık ilişkileri şeklinde, sütunlar hizalı biçimde saklanır.

## Özellikler
- **Kullanıcı Yönetimi:** Yeni kullanıcı ekleme, kullanıcıların kimlik bilgileri ve isimlerinin saklanması.
- **Arkadaşlık Yönetimi:** İki kullanıcı arasında arkadaşlık ilişkisi tanımlama ve ortak arkadaşların tespiti.
- **İlişki Analizi:**
  - DFS kullanarak iki kullanıcı arasında ilişki (arkadaşlık) yolu mevcut mu?
  - Topluluk tespiti: Sistemde bulunan bağlı bileşenlerin (toplulukların) belirlenmesi.
  - Etki alanı hesaplama: Bir kullanıcının erişebildiği (doğrudan veya dolaylı) kullanıcı sayısının hesaplanması.
- **Red–Black Tree Uygulaması:** Veri yapısının verimli ve dengeli saklanması için Red–Black Tree algoritmasının uygulanması.
- **Düzenli Dosya Yazımı:** `veriseti.txt` dosyası, iki bölüme ayrılarak saklanır:
  - "KULLANICI BİLGİLERİ" bölümü: Kullanıcıların ID ve isim bilgileri.
  - "ARKADAŞLIK İLİŞKİLERİ" bölümü: Arkadaşlık ilişkileri duplicate (tekrarlı) kayıtlar olmadan listelenir.

## Dosya Yapısı
Projenin temel dosya yapısı aşağıdaki gibidir:
------------------------------------------------
Social-Network-Analysis  
├── src  
│   ├── main.c           # Tüm ana kod (kullanıcı yönetimi, analiz, Red–Black Tree ve etkileşimli mod)  
├── veriseti.txt         # Projede kullanılan veri kaydı dosyası  
├── README.md            # Bu açıklama dosyası  
└── Makefile             # (Opsiyonel) Derleme ayarları

## Kurulum ve Kullanım
1. **Derleme:**
   Terminalden GCC kullanarak projenizi derleyebilirsiniz:
   
       gcc -o sosyal_ag main.c

   Veya Makefile varsa aşağıdaki komutla derleyin:
   
       make

2. **Çalıştırma:**
   
       ./sosyal_ag

## Veri Kaydı Formatı (veriseti.txt)
Veriler dosyaya aşağıdaki düzenli formatta yazılır:

========== KULLANICI BİLGİLERİ ==========
Tür        ID    İsim
-----------------------------------------
USER       1     Ahmet
USER       2     Ayşe
USER       3     Mehmet

========== ARKADAŞLIK İLİŞKİLERİ ==========
Tür        ID1   ID2
-----------------------------------------
FRIEND     1     2
FRIEND     1     3
FRIEND     2     3

## Katkıda Bulunma
1. Depoyu forklayın ve yerel olarak klonlayın.  
2. Yeni bir gelişme dalı oluşturun:
   
       git checkout -b yenilik-dali

3. Yapacağınız değişiklikleri commit edip gönderin:
   
       git push origin yenilik-dali

4. Pull request açarak önerilerinizi paylaşın.



## Geliştirici
Proje **Harun Berke Öztürk** tarafından geliştirilmiştir.  
Detaylı bilgi ve iletişim için GitHub profilimi ziyaret edin:  
[GitHub Profilim](https://github.com/Suylakan)
