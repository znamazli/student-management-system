# Tələbə İdarəetmə Sistemi

Bu, C dilində yazdığım kiçik konsol proqramıdır. Ideya sadədir: tələbələrin siyahısını bir yerdə saxlamaq, lazım olanda yeni tələbə əlavə etmək, məlumatını dəyişmək və ya silmək. Proqramı bağlayanda məlumatlar itmir, çünki hamısı `students.txt` faylına yazılır və növbəti açılışda oradan oxunur.

## Menyu

Proqramı açanda 5 seçim çıxır, rəqəm yazıb Enter basırsan:

1. **Tələbə əlavə et** – ad, soyad, ixtisas və GPA daxil edirsən. ID-ni proqram özü verir, ona görə iki tələbənin eyni ID-si ola bilməz.
2. **Tələbəni sil** – ID-ni yazırsan, həmin tələbə siyahıdan və fayldan silinir.
3. **Tələbələrin siyahısı** – hamı cədvəl şəklində ekrana çıxır. Siyahı boşdursa, bu barədə mesaj verir.
4. **Tələbəni yenilə** – ID ilə tələbəni seçirsən, sonra dəyişmək istədiyin sahəni yazırsan. Dəyişmək istəmədiyin sahəni boş buraxıb Enter basmaq kifayətdir.
5. **Çıx** – proqram bağlanır, yaddaş təmizlənir.

Yanlış şey yazsan (məsələn rəqəm yerinə hərf), proqram çökmür, sadəcə xəta yazıb yenidən soruşur.

## Necə kompilyasiya olunur

Kompüterdə `gcc` olmalıdır. Qovluğun içində terminal açıb bunu yaz:

```
gcc main.c -o main.exe -Wall -Wextra
```

İşə salmaq üçün (Windows PowerShell):

```
.\main.exe
```

## Fayl formatı

Hər tələbə `students.txt`-də bir sətirdir, sahələr `|` ilə ayrılır:

```
1|Zehra|Namazli|Komp muh|3.85
```

Fayl yoxdursa, proqram ilk tələbə əlavə olunanda özü yaradır.