# Féléves fealdat: Minotaurs Labyrinth

## Felhasznált rendszerek:C nyelv, SDL2 keretrendszer, OpenGL grafikus API.

A projekt egy belső nézetű (FPS) játék ahol egy procedurálisan generált labirintusrendszert valósít meg "Modular level design-al". A játékos célja az útvesztő felfedezése, miközben el kell kerülnie Minósz király elátkozott fiát. A labirintus előre modellezett szobaelemekből épül fel, amelyek dinamikusan töltődnek be és ürülnek ki a memóriából a játékos mozgása alapján.

## 2. Alapvető funkciók (Minimum követelmények)

**Kamerakezelés:** (FPS)

**Objektumok:** .obj fájlbol betöltött minotaurusz modell.

**Animáció:** Fáklya animáció és kard lebegés.

**Textúrázás:** Minden modell saját textúrával rendelkezik (kőfalak, talaj, minotaurusz).

**Világítás:** Statikus fényforrás (fáklya effektus), amelynek intenzitása a + és - gombokkal szabályozható.

**Súgó:** Az F1 billentyű megnyomásával előhívható kezelési útmutató.

## 3. Választott többlet funkciók (5 darab)
**Ütközésvizsgálat (Bounding-box):** ütközéskezelés a játékos és a falak között.

**AI (Ellenfelek vezérlése):** Egy egyszerű algoritmus, amely a Minotaurusz karaktert a játékos felé mozgatja, ha az egy adott távolságon belülre kerül.

**Köd (Fog):** Szoftveres köd effekt.

**Procedurális geometria:** A labirintus alaprajzának véletlenszerű generálása minden indításkor, a moduláris szobaelemek variálásával. Ezáltal mindig új játék élményt nyújtva a felhasználónak.

**Átlátszóság (Transparency):** Alpha blending használata a láng-effektekhez.