🚀 Alternate History Simulator Geliştirme Yol Haritası
Proje Tanımı: C++ tabanlı CLI aracı—İngilizce input'larla "what-if" tarih simülasyonu yapar (örn. "USA atom bomb does not develop" → "USA No Nuke" flag'iyle zincirleme olaylar). 3 haftalık (21 gün) plan, 4-6 saat/gün. CMake ile cross-platform, GitHub entegrasyonlu.
Genel Kurallar:

Her gün: Tasks tamamla, test et, commit/push et ("feat: gün-adı").
Araçlar: VS 2022 (Console App CMake), CMake, g++ (C++20).
Test: Her gün en az 3 run, GitHub'a güncelle.
Kaynak: cppreference.com (C++ docs), Wikipedia (CSV verileri).
Başlangıç: Gün 0—repo hazır, README.md ve CMakeLists.txt var.

📅 Günlük Yapı
Her gün için: Başlangıç Durumu (dosya yapısı), Hedef, Tasks (adım adım), Son Durum, Süre, Milestone (başarı kriteri).
Hafta 1: Temel Yapı (Gün 1-7)

Gün 1: Input Parsing

Başlangıç: Boş main.cpp.
Hedef: İngilizce input oku, ülkeyi parse et (örn. "usa..." → "USA").
Tasks: Include'lar ekle (iostream, string, sstream, algorithm); getline ile satır oku; istringstream ile kelime ayır; transform ile ilk kelimeyi uppercase yap; "Parsed Country: USA" yazdır; boş input hatası ver; 3 test run.
Son Durum: src/main.cpp (parse kodu), CMakeLists.txt (add_executable sim src/main.cpp).
Süre: 4s. Milestone: Run input alır, "USA" gösterir. Commit: "feat: basic parse".


Gün 2: Flag Generation

Başlangıç: Parse hazır.
Hedef: Flag üret (örn. "does not" → "No Nuke").
Tasks: utils.h/c++.cpp ekle; negativeWords map (5 kelime); eventKeywords map (3 kelime); generateFlag fonksiyonu yaz; main'de çağır, 3 input test et.
Son Durum: src/utils.{h,cpp}, main flag çağırır.
Süre: 4s. Milestone: 3 flag doğru. Commit: "feat: flag gen".

Gün 3: CSV Loading

Başlangıç: Flag hazır.
Hedef: CSV yükle (historicalEvents map).
Tasks: Event struct ekle (name, prob); loadFromCSV: ifstream parse; data/events.csv oluştur (5 İngilizce satır); main'de yükle, map yazdır.
Son Durum: data/events.csv, utils + Event/loadFromCSV.
Süre: 5s. Milestone: 5 event göster. Commit: "data: csv load".

Gün 4: Event Categorization

Başlangıç: CSV hazır.
Hedef: Event'leri kategorize et.
Tasks: Event.category ekle; categorizeEvent: keyword tarama; templates pool (5 kategori, 3 twist); yüklerken kategorize; 5 test.
Son Durum: events.csv + Category sütunu; utils + categorize.
Süre: 4s. Milestone: Kategoriler doğru. Commit: "feat: categorize".

Gün 5: RNG & Decay

Başlangıç: Kategoriler hazır.
Hedef: Olasılık decay ve RNG uygula.
Tasks: applyDecay (pow(0.92, years)); shouldMutate (mt19937 RNG); main'de 10 yıl decay test; 5 run.
Son Durum: utils + decay/RNG; main test.
Süre: 4s. Milestone: Decay doğru. Commit: "feat: decay RNG".

Gün 6: Mutation Text

Başlangıç: RNG hazır.
Hedef: Dinamik text mutate.
Tasks: generateMutationText: name split, RNG twist pool; flag prefix; 5 event test.
Son Durum: utils + mutation; tests/simple_test.cpp (5 test).
Süre: 5s. Milestone: Text mantıklı. Commit: "feat: mutation text".

Gün 7: Hafta 1 Entegrasyon

Başlangıç: Mutation hazır.
Hedef: Temel akış birleştir.
Tasks: Main: input→flag→load→mutate 1 event; 10 test; yorumlar ekle.
Son Durum: tests/simple_test; CMake test target.
Süre: 4s. Milestone: Akış hatasız. Commit: "integrate: week1".



Hafta 2: Simülasyon Çekirdeği (Gün 8-14)

Gün 8: Recursion Core

Başlangıç: Entegre.
Hedef: Yıl adımlı recurse.
Tasks: simulation.h/c++.cpp ekle; simulateRecursive (+5 yıl, prob, flag, prefix); tree indent; 1914-1950 test (3 level).
Son Durum: src/simulation.{h,cpp}.
Süre: 5s. Milestone: Tree doğru. Commit: "feat: recursion".


Gün 9: Chain Effects

Başlangıç: Recurse temel.
Hedef: Flag zincirleme mutate.
Tasks: Flag geçir/mutate (örn. No Nuke→war*0.7); effects map; 2 level test; decay entegre.
Son Durum: simulation + chain.
Süre: 5s. Milestone: Etkiler uygula. Commit: "feat: chains".


Gün 10: Mutasyon/Filtre Entegrasyon

Başlangıç: Chain hazır.
Hedef: Recurse'da mutate filtrele.
Tasks: RNG mutate çağır; flag prob filtre (No War→war*0.5); 5 event test; CSV cat kullan.
Son Durum: events.csv (50+ satır).
Süre: 4s. Milestone: Tetikleme çalışır. Commit: "integrate: filter".


Gün 11: Seeded RNG

Başlangıç: Entegre hazır.
Hedef: Tekrarlanabilir RNG.
Tasks: Seed gen (main param); depth>50 return; seed42 test; Monte stub (5 avg).
Son Durum: utils + seeded.
Süre: 4s. Milestone: Aynı run. Commit: "feat: seeded RNG".

Gün 12: File Export

Başlangıç: Seeded hazır.
Hedef: Tree'yi dosyaya kaydet.
Tasks: exportToTxt (ofstream, lines vec); recurse'da topla; main çağır; output.txt test.
Son Durum: output/ (üretilen); utils + export.
Süre: 5s. Milestone: Dosya okunaklı. Commit: "feat: export".

Gün 13: Multi-Country

Başlangıç: Export hazır.
Hedef: 5 ülke profili.
Tasks: countries.csv ("Country,tech_boost,war_delay"); yükle/recurse uygula; 5 input test; <5s perf.
Son Durum: data/countries.csv; utils + loader.
Süre: 5s. Milestone: Multi run. Commit: "feat: multi-country".

Gün 14: Hafta 2 Entegrasyon

Başlangıç: Multi hazır.
Hedef: Tam akış.
Tasks: Main args (event/year/end); flag→recurse→export; 20 test; yorumlar.
Son Durum: tests/full_test; CMake + test.
Süre: 4s. Milestone: Tam run. Commit: "integrate: week2".



Hafta 3: Polish & Final (Gün 15-21)

Gün 15: CLI Menu & Options

Başlangıç: Tam hazır.
Hedef: Menü input ve komut satırı seçenekleri.
Tasks:
- Implement interactive CLI menu (`src/cli.{h,cpp}`) and non-interactive argument parsing in `main`.
- Add and parse the following command-line options: `--seed <N>` (seed RNG), `--monte <M>` (Monte Carlo runs), `--export <txt|json>` (output format), `--log <level>` (none/info/debug).
- Validate arguments, show `--help` summary, and fallback to interactive mode when no args provided.
- Add unit tests for parsing and sample runs (3 tests).
Son Durum: src/cli.{h,cpp} + main arg parsing.
Süre: 5s. Milestone: Menu ve CLI seçenekleri çalışır. Commit: "feat: cli menu & options".

Gün 16: Monte Carlo

Başlangıç: Menü hazır.
Hedef: Çoklu run ortalaması.
Tasks: monteCarlo(runs,flag): avg prob; rapor ("Likely: 60%"); export ekle; 50 test.
Son Durum: simulation + monte.
Süre: 5s. Milestone: Rapor ok. Commit: "feat: monte carlo".

Gün 17: Hata & Log

Başlangıç: Monte hazır.
Hedef: Sağlamlaştır.
Tasks: Try-catch; logToFile (ofstream log.txt); Valgrind; kötü input handle.
Son Durum: logs/; utils + log.
Süre: 4s. Milestone: Çökmez. Commit: "fix: error log".

Gün 18: Cross-Platform

Başlangıç: Log hazır.
Hedef: Win/Linux.
Tasks: CMake geliştir; #ifdef WIN32 path; Makefile; 2 OS test.
Son Durum: build.sh; CMake cross.
Süre: 4s. Milestone: Derleme. Commit: "build: cross".

Gün 19: Docs

Başlangıç: Build hazır.
Hedef: Tam docs.
Tasks: README kurulum/örnek/UML text; Doxygen yorum; LinkedIn taslak; video genişlet.
Son Durum: docs/UML.txt; README tam.
Süre: 4s. Milestone: Docs hazır. Commit: "docs: full".

Gün 20: API Bonus

Başlangıç: Docs hazır.
Hedef: Curl fetch.
Tasks: CMake + curl; fetchAPI (Wikipedia İngilizce); CSV alt yükle; "WWII" test.
Son Durum: src/api.cpp; CMake + curl.
Süre: 6s. Milestone: Çekme. Commit: "feat: api bonus".

Gün 21: Final Release

Başlangıç: Bonus hazır.
Hedef: Bitir.
Tasks: Test/fix; GitHub v1.0; CV ekle; arkadaş test.
Son Durum: release/v1.0 zip.
Süre: 3s. Milestone: Canlı! Commit: "release: v1.0".



İpuçları: Günlük milestone'ı tamamla, VS Git Changes ile commit/push. AI'ya sor: "ROADMAP.md'ye göre Gün X tasks'ını detaylandır." Başarılar—tarihi kodla değiştir! 🌍