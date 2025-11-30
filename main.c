#include <stdio.h>   
#include <stdlib.h>  // rand, srand fonksiyonları için (oyunun tahmin edilebilirliğini azaltmak için)
#include <time.h>    // time fonksiyonu için (rand seed ayarlamak için)

int main() {

    // Oyuncuya ait bilgilerin tutulduğu değişkenler
    int health = 100;     // Oyuncunun sağlığı (0-100 arası değer alabilir)
    int energy = 100;     // Oyuncunun enerjisi (0-100 arası değer alabilir)
    int food = 2;         // Oyuncunun sahip olduğu yemek sayısı (her av ile yemek artacak)
    int hasShelter = 0;   // Oyuncunun sığınağı var mı? (0 = yok, 1 = var)

    char command;         // Kullanıcının gireceği komutu tutan karakter
    int running = 1;      // Oyun döngüsünü kontrol eden değişken (1 = devam et, 0 = bitir)

    // time(NULL) o anki zamanı saniye cinsinden verir, srand sayesinde her çalıştırmada farklı (rastgele) sonuç üretilir
    srand(time(NULL));

    // Başlangıç mesajı
    printf("Hayatta Kalma Oyununa Hosgeldiniz!\n");
    printf("Komutlar: A=Avlan, S=Siginak, R=Dinlen, E=Durum, F=Tehlike Dalgasi, P=Gecis, X=Cikis\n");

    // Ana oyun döngüsü: "Kullanıcı X karakterine basıncaya kadar çalışsın" şeklinde 
    // En az bir defa komutu çalıştırmak istediğimiz için do-while döngüsü
    do {
        // Her tur başında durumun gösterilmesi
        printf("\n--- MEVCUT DURUM ---\n");
        printf("Saglik: %d | Enerji: %d | Yemek: %d | Siginak: %s\n",
               health, energy, food, hasShelter ? "Var" : "Yok");

        // Kullanıcıdan komut alınması
        printf("\nKomut girin (A,S,R,E,F,P,X): ");
        scanf(" %c", &command); 

        // Komuta göre ne yapılacağının belirlenmesi için switch-case
        switch (command) {

            // A veya a girildiyse: Avlanma
            case 'A':
            case 'a':
                printf("\n[AVLANMA] Ormana dogru ilerliyorsunuz...\n");

                // Avlanmak enerji harcatacağından aritmetik operatör kullanarak sağlık puanının azaltılması
                energy = energy - 15;

                // Ormanda ava denk gelip gelemeyeceğimizi bilmiyoruz, bu nedenle rastgele sayı üreterek şansa bakalım (0-99 arası)
                int chanceA;
                chanceA = rand() % 100;

                // If-else ile olası durumları yani şansımızı kontrol edelim
                if (chanceA < 50) {
                    // Oyuncu %50 ihtimalle yemek buluyor demektir
                    food = food + 1;
                    printf("Bir hayvan avladiniz! Yemek +1\n");
                } else if (chanceA < 75) {
                    // Oyuncu %25 ihtimalle yaralandı demektir
                    health = health - 15;
                    printf("Av sirasinda yaralandiniz! Saglik -15\n");
                } else {
                    //Oyuncu %25 ihtimalle eli boş dönecek demektir
                    printf("Aradiniz ama bir av bulamadiniz.\n");
                }

                // Av arama sırasında yaralanma ve av bulamama durumunda enerji biterse oyun biter
                if (energy <= 0) {
                    printf("Enerjiniz tukendi. Oyun bitti.\n");
                    running = 0; // oyun döngüsünü bitir
                }
                break; // A komutu bitti

            // S veya s: Sığınak arama
            case 'S':
            case 's':
                printf("\n[SIGINAK ARAMA] Ormani incelemeye basladiniz...\n");

                int luck;       // Sığınak bulabilmek için enerjiye ve şansa ihtiyaç var
                int danger;     // Ayrıca sığınak bulmak için çevrede tehlike olmamalı

                luck = rand() % 100;   // 0-99 arası şans
                danger = rand() % 3;   // 0,1 veya 2

                // Oyuncu ya sığınak bulur ya bulamaz, dolayısıyla mantıksal operatörler: && ve || kullanılmalı
                // Koşul: Enerji 50'den büyük VE şans 40'tan büyükse YA DA tehlike 0 ise
                if ( (energy > 50 && luck > 40) || danger == 0 ) {
                    hasShelter = 1; // artık sığınağı var
                    printf("Tebrikler, guvenli bir siginak buldunuz!\n");
                } else {
                    printf("Uygun bir siginak bulamadiniz. Biraz daha yoruldunuz.\n");
                    energy = energy - 5;
                }
                break; // S komutu bitti

            // R veya r: Dinlenme
            case 'R':
            case 'r':
                printf("\n[DINLENME] Bir sure dinleniyorsunuz...\n");

                // Oyunda bazı eylemler enerjiyi azaltırken, bazı eylemlerin de enerjiyi yerine koyması gerek
                // Eger oyuncunun sığınağı varsa daha iyi dinlenerek enerjisini daha hızlı toplayabilir
                if (hasShelter == 1) {
                    energy = energy + 20;
                    health = health + 10;
                    printf("Siginakta dinlendiniz. Enerji +20, Saglik +10\n");
                } else {
                    energy = energy + 10;
                    printf("Acil alanda dinlendiniz. Enerji +10\n");
                }

                // Sağlık ve enerji 100'u geçmesin diye kontrol etmek gerekli 
                // (oyun daha da geliştiğinde bu ek enerji ile ilgili farklı eylemler de eklenebilir)
                if (energy > 100) {
                    energy = 100;
                }
                if (health > 100) {
                    health = 100;
                }
                break; // R komutu bitti

            // E veya e: Durumu/Envanteri goster
            case 'E':
            case 'e':
                printf("\n[DURUM GORUNTULE]\n");
                printf("Saglik: %d\n", health);
                printf("Enerji: %d\n", energy);
                printf("Yemek: %d\n", food);
                printf("Siginak: %s\n", hasShelter ? "Var" : "Yok");
                break; // E komutu bitti

            // F veya f: Oyun sırasında oyuncunun karşılaşacağı çeşitli tehlike dalgası simülasyonları için For Döngüsü
            case 'F':
            case 'f':
                printf("\n[TEHLIKE DALGASI] Bolgeye bir dizi tehlike yaklasiyor!\n");

                int i; // for döngüsündeki sayaç

                // Dalga tehlike simülasyonunda 3 durum söz konusu (sağlığın etkilenmesi, enerjinin etkilenmesi ya da zararsız atlatma)
                for (i = 1; i <= 3; i = i + 1) {
                    printf("\n%d. dalga geliyor...\n", i);

                    int event;     // o dalgadaki olayın karşılığı olan sayı
                    event = rand() % 3; // 0, 1 veya 2 durumlarından biri gerçekleşecek

                    if (event == 0) {
                        // Saglık kaybı
                        health = health - 10;
                        printf("Bir yaratik size saldirdi! Saglik -10\n");
                    } else if (event == 1) {
                        // Enerji kaybı
                        energy = energy - 10;
                        printf("Kacinmaya calistiniz. Enerji -10\n");
                    } else {
                        // Zararsız dalga
                        printf("Bu dalgada size zarar veren bir sey olmadi.\n");
                    }

                    // Her dalga sonunda karakter ölmüş mü kontrol edilmeli
                    if (health <= 0) {
                        printf("Tehlike dalgalari nedeni ile maalesef karakteriniz oldu.\n");
                        running = 0; // oyun bitecek
                        break;       // for döngüsünden çıkış
                    }
                }
                break; // F komutu bitti

            // P veya p: DO-WHILE ile geçiş mekanizması (şifreli kapı gibi)
            case 'P':
            case 'p':
                printf("\n[GECIS NOKTASI] Onunuzde bir gecit var. Acmak icin dogru sifreyi girin.\n");

                char password; // Şifre için kullanıcının girdigi karakter

                // En az bir kere sorulması ve doğru karakter girilene kadar devam edilmesi için yine do-while döngüsü
                // Diyelim ki doğru karakter (key = anahtar kelimesinin ilk harfi) 'K' olsun
                do {
                    printf("Gecmek icin sorunun yanitini girin 'Kapilari acan seyin ilk harfi nedir?' : ");
                    scanf(" %c", &password);
                } while (password != 'K' && password != 'k'); // doğru değilse tekrar sorulur

                printf("Dogru sifre! Kapidan basariyla gectiniz.\n");
                break; // P komutu bitti

            // X veya x: Programdan çıkış
            case 'X':
            case 'x':
                printf("\n[Cikis] Oyunu sonlandirdiniz.\n");
                running = 0; // döngüyü bitir
                break;

            // Tanınmayan komutlar için default tepki
            default:
                printf("\nGecersiz komut girdiniz. Lutfen A,S,R,E,F,P veya X kullanin.\n");
                break;
        }

        // Her turun sonunda ekstra ölüm kontrolü
        if (health <= 0) {
            printf("\nSagliginiz sifirin altina dustu. Oyun sona erdi.\n");
            running = 0;
        }

        if (energy <= 0) {
            printf("\nEnerjiniz kalmadi. Daha fazla devam edemiyorsunuz. Oyun sona erdi.\n");
            running = 0;
        }

    } while (running == 1); // running 1 oldugu sürece oyun döner

    printf("\nSimulasyon sonlandi. Tesekkürler!\n");

    return 0; // Program başarıyla bitti
}
