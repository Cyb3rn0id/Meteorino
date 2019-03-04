/***********************************************
 * 
 * METEORINO (c)2018 Giovanni Bernardo
 * 
 * https://www.settorezero.com
 * https://www.facebook.com/settorezero
 * 
 * Calendar / Saint of the day / Birthday list
 * (anniversaires per day)
 * Separate waste collection (weekly)
 * 
 * This file derived from my old ORbit16 project:
 * https://www.settorezero.com/orbit16/home/projects/table-calendar-with-time-date-temperature-and-saint-of-the-day/
 * 
 * A lot of work for this file
 * PLEASE DO NOT REDISTRIBUTE!
 * 
 ***********************************************/

// Day of week names
String DayOfWeek[]={"DOMENICA","LUNEDI","MARTEDI","MERCOLEDI","GIOVEDI","VENERDI","SABATO"};
//String DayOfWeek[]={"SUNDAY","MONDAY","TUESDAY","WEDNESDAY","THURSDAY","FRIDAY","SATURDAY"};

// word for "tomorrow:", used by separate waste collection indication
String tom="Domani: ";

// word for "today:", used by separate waste collection indication
String tod="Oggi: ";

// word for waste collection frame title
String swc="DIFFERENZIATA";

// show/don't show easter sentence when is Easter
bool showEaster=true;

// sentence for easter day
String eas="SANTA PASQUA";

// personalized sentence for weekday, used for separate waste collection.
// category of waste is intended for present day. Longest sentence is 15 chars long
const char fw[8][16]={
            "niente",         // today is Sun/Dom
            "Umido",          // today is Mon/Lun
            "Plastica",       // today is Tue/Mar 
            "Umido",          // today is Wed/Mer
            "Indifferenziato",// today is Thu/Gio
            "Carta/vetro",    // today is Fri/Ven
            "Umido"           // today is Sat/Sab
            };

// Start index for every month in array - DO NOT TOUCH!!
const int startIndex[]={0,31,60,91,121,152,182,213,244,274,305,335};

// Saint/Birthday/whatever you want list
// 1 anniversary for each day. Longest sentence is 20 chars long
const char dp[366][21]={
	// JANUARY *******************************************************************************
	"Maria Madre di Dio",         // 1
	"S.Basilio, Gregorio",        // 2
	"Santa Genoveffa",            // 3
	"S. Elisabetta Anna",         // 4
	"Santa Amelia",               // 5
	"Epifania di N.S.",           // 6
	"San Raimondo",               // 7
	"S. Massimo di Pavia",        // 8
	"San Giuliano",               // 9
	"San Aldo",                   // 10
	"San Igino papa",             // 11
	"S. Cesira di Arles",         // 12
	"S. Ilario vescovo",          // 13
	"S. Felice da Nola",          // 14
	"San Mauro abate",            // 15
	"S. Marcello I papa",         // 16
	"S. Antonio abate",           // 17
	"Santa Margherita",           // 18
	"SS. Mario e Marta",          // 19
	"S.Sebastiano,Fabiano",       // 20
	"Sant' Agnese",               // 21
	"San Gaudenzio",              // 22
	"Sant' Emerenziana",          // 23
	"S. Francesco vescovo",       // 24
	"Conv. di San Paolo",         // 25
	"SS. Tito e Timoteo",         // 26
	"S. Angela Merici",           // 27
	"S. Tommaso d'Aquino",        // 28
	"San Costanzo",               // 29
	"Santa Martina",              // 30
	"San Ciro",                   // 31
	
	// FEBRUARY ******************************************************************************
	"S. Verdiana,Brigida",        // 1
	"Pres. del Signore",          // 2
	"S. Biagio, Oscar",           // 3
	"Sant' Andrea",               // 4
	"Sant' Agata",                // 5
	"San Paolo Miki",             // 6
	"S. Lorenzo, Teodoro",        // 7
	"San Girolamo",               // 8
	"Santa Apollonia",            // 9
	"Santa Scolastica",           // 10
	"N.S. di Lourdes",            // 11
	"S. Eulalia, Marina",         // 12
	"S. Benigno, Fosca",          // 13
	"S. Valentino,Cirillo",       // 14
	"S. Faustino,Giovita",        // 15
	"S. Iuliana, Elia",           // 16
	"San Flaviano",               // 17
	"S. Costanza,Massimo",        // 18
	"S. Mansueto,Corrado",        // 19
	"San Eleuterio",              // 20
	"S. Pier Damiani",            // 21
	"Santa Margherita",           // 22
	"San Policarpo",              // 23
	"S. Sergio, Lucio",           // 24
	"S. Nestore, Cesario",        // 25
	"San Porfirio",               // 26
	"S. Leandro, Gabriele",       // 27
	"San Romano",                 // 28
	"San Augusto",                // 29
	
	// MARCH *********************************************************************************
	"S. Albino, David",           // 1
	"San Simplicio",              // 2
	"S. Tiziano, Caterina",       // 3
	"San Casimiro",               // 4
	"S. Adriano, Olivia",         // 5
	"San Marziano",               // 6
	"S. Perpetua,Felicita",       // 7
	"S. Giovanni di Dio",         // 8
	"S. Francesca romana",        // 9
	"S. Maria Eugenia",           // 10
	"San Costantino",             // 11
	"San Nicodemo",               // 12
	"San Salomone",               // 13
	"Santa Matilde",              // 14
	"S. Luisa, Cesare",           // 15
	"S. Eusebio, Ilario",         // 16
	"San Patrizio",               // 17
	"San Cirillo",                // 18
	"San Giuseppe",               // 19 - In Italy it's the Father's day
	"Santa Claudia",              // 20
	"S. Elia, Benedetto",         // 21
	"San Benvenuto",              // 22
	"San Turibio",                // 23
	"S.Caterina di Svezia",       // 24
	"Annunciazione di NS",        // 25
	"S. Emanuele, Lucia",         // 26
	"San Ruperto",                // 27
	"S. Sisto, Gontrano",         // 28
	"S. Jonah, Bertoldo",         // 29
	"S. Amedeo, Irene",           // 30
	"S. Stephen, Amos",           // 31
	
	// APRIL ********************************************************************************
	"S. Venenzio, Ugo",           // 1
	"S.Francesco di Paola",       // 2
	"S. Luigi, Riccardo",         // 3
	"S. Isidoro",                 // 4
	"S. Vincenzo Ferrer",         // 5
	"Santa Prudemce",             // 6
	"S. Giovanni B. dls",         // 7
	"S. Dionigi, Giulia",         // 8
	"Santa Casilda",              // 9
	"San Ezechiele",              // 10
	"San Stanislao",              // 11
	"S. Giulio, Zeno",            // 12
	"S. Martino I",               // 13
	"S. Raoul, Liduina",          // 14
	"San Telmo",                  // 15
	"Santa Bernadetta",           // 16
	"Sant'Elia",                  // 17
	"San Calogero",               // 18
	"San Ermogene",               // 19
	"S. Sara, Teodoro",           // 20
	"San Anselmo",                // 21
	"San Sotero",                 // 22
	"S.Giorgio, Adalberto",       // 23
	"San Fedele",                 // 24
	"S. Marco Evangelista",       // 25 - In Italy it's "Anniversario della liberazione"
	"S. Marcellino",              // 26
	"Santa Zita",                 // 27
	"S. Pietro, Valeria",         // 28
	"Santa Antonella",            // 29
	"San Pio V papa",             // 30
	
	// MAY ********************************************************************************
	"San Giuseppe artig.",        // 1 - In Italy it's the Worker's day
	"S. Viborada,Atanasio",       // 2
	"S. Filippo e Giacomo",       // 3
	"San Ciriaco",                // 4
	"S. Angelo, Gottardo",        // 5
	"San Lucio di Cirene",        // 6
	"S.Benedetto,Virginia",       // 7
	"San Vittore",                // 8
	"San Pacomio",                // 9
	"S. Cataldo, Alfio",          // 10
	"San Fabio",                  // 11
	"S. Nereo, Achilleo",         // 12
	"N.S. di Fatima",             // 13
	"S. Mattia apostolo",         // 14
	"Sant' Isidoro",              // 15
	"S. Ubaldo, Gemma",           // 16
	"S. Paquale Baylon",          // 17
	"San Leonardo",               // 18
	"San Celestino",              // 19
	"San Bernardino",             // 20
	"San Cristoforo",             // 21
	"Santa Rita da Cascia",       // 22
	"S. Eutizio, Fiorenzo",       // 23
	"S.Maria Ausiliatrice",       // 24
	"San Beda",                   // 25
	"San Filippo Neri",           // 26
	"Santa Restituta",            // 27
	"San Just, Emilio",           // 28
	"Sant' Agostino",             // 29
	"S. Giovanna D'Arco",         // 30
	"Visitazione B.V.M.",         // 31
	
	// JUNE ********************************************************************************
	"San Giustino",               // 1
	"S. Marcellino,Pietro",       // 2 In Italy it's the "Anniversario della Repubblica"
	"S. Carlo Lwanga",            // 3
	"S. Francesco Caracc.",       // 4
	"San Bonifacio",              // 5
	"San Norberto",               // 6
	"San Candido",                // 7
	"San Massimino",              // 8
	"Sant'Efrem il Siro",         // 9
	"Santa Diana",                // 10
	"San Barnaba",                // 11
	"San Onofrio, Guido",         // 12
	"S. Antonio da Padova",       // 13
	"Sant'Eliseo",                // 14
	"Santa Germana",              // 15
	"San Quirico",                // 16
	"S. Ranieri, Manuele",        // 17
	"Sant'Erasmo, Marina",        // 18
	"S. Gervasio,Protasio",       // 19
	"S. Ettore, Silverio",        // 20
	"S. Luigi Gonzaga",           // 21
	"San Tommaso Moro",           // 22
	"San Lanfranco",              // 23
	"S. Giovanni Battista",       // 24
	"S.Guglielmo,Febronia",       // 25
	"San Josemaria",              // 26
	"San Cirillo di Ales.",       // 27
	"Sant'Ireneo di Lione",       // 28
	"S. Pietro e Paolo",          // 29
	"Santi primi martiri",        // 30
	
	// JULY ********************************************************************************
	"S. Aronne, Ester",           // 1
	"Sant' Ottone",               // 2
	"San Tommaso apostolo",       // 3
	"S. Elisabetta",              // 4
	"S.Antonio Maria Zac.",       // 5
	"Santa Maria Goretti",        // 6
	"S. Claudio, Edda",           // 7
	"S. Aquila, Priscilla",       // 8
	"Sant' Armando",              // 9
	"San Silvano",                // 10
	"S. Benedetto da Nor.",       // 11
	"San Fortunato",              // 12
	"Sant'Enrico, Dario",         // 13
	"S. Camillo de Lellis",       // 14
	"San Bonaventura",            // 15
	"BVM del Carmelo",            // 16
	"Sant' Alessio",              // 17
	"San Calogero",               // 18
	"San Simmaco",                // 19
	"Sant' Apollinare",           // 20
	"San Lorenzo da Brin.",       // 21
	"S. Maria Maddalena",         // 22
	"S. Brigida di Svezia",       // 23
	"Santa Cristina",             // 24
	"San Giacomo apostolo",       // 25
	"S. Gioacchino e Anna",       // 26
	"S. Liliana, Aurelio",        // 27
	"S. Nazario e Celso",         // 28
	"S. Marta di Betania",        // 29
	"San Pietro Crisologo",       // 30
	"Sant' Ignazio",              // 31
	
	// AUGUST ********************************************************************************
	"Sant' Alfonso",              // 1
	"Sant' Eusebio",              // 2
	"Sante Mara e Lidia",         // 3
	"San Nicodemo",               // 4
	"Sant'Osvaldo",               // 5
	"Trasfig. del Signore",       // 6
	"San Gaetano",                // 7
	"San Domenico",               // 8
	"San Romano",                 // 9
	"San Lorenzo",                // 10
	"Santa Chiara",               // 11
	"Sant'Euplio",                // 12
	"S. Ponziano,Ippolito",       // 13
	"S. Alfredo",                 // 14
	"Assunzione di Maria",        // 15
	"San Rocco",                  // 16
	"San Giacinto",               // 17
	"Sant'Elena",                 // 18
	"S. Ludovico, Italo",         // 19
	"San Bernardo abate",         // 20
	"San Pio X",                  // 21
	"B.V. Maria Regina",          // 22
	"Santa Rosa da Lima",         // 23
	"San Bartolomeo",             // 24
	"San Ludovico",               // 25
	"Sant' Oronzo",               // 26
	"Santa Monica, Anita",        // 27
	"Sant'Agostino",              // 28
	"Martirio S. Giovanni",       // 29
	"San Faustina",               // 30
	"Sant' Aristide",             // 31
	
	// SEPTEMBER *****************************************************************************
	"Sant' Egidio",               // 1
	"Sant' Elpidio",              // 2
	"San Gregorio Magno",         // 3
	"Santa Rosalia",              // 4
	"San Lorenzo",                // 5
	"San Umberto",                // 6
	"Santa Regina",               // 7
	"Nat. Beata V. Maria",        // 8
	"San Sergio",                 // 9
	"San Nicola Tolentino",       // 10
	"S. Ramiro, Emiliano",        // 11
	"S. Tasauro, Guido",          // 12
	"San Maurilio",               // 13
	"Esalt. Santa Croce",         // 14
	"B.V.M. Addolorata",          // 15
	"S. Cornelio,Cipriano",       // 16
	"S. Roberto",                 // 17
	"S. Sofia, Arianna",          // 18
	"San Gennaro",                // 19
	"Santa Candida",              // 20
	"San Matteo apostolo",        // 21
	"San Maurizio",               // 22
	"S.Pio da Pietrelcina",       // 23
	"San Pacifico",               // 24
	"San Nicolao, Aurelia",       // 25
	"Santi Cosma, Damiano",       // 26
	"S.Vincenzo de' Paoli",       // 27
	"San Venceslao",              // 28
	"S. Michele,Gabriele",        // 29
	"San Girolamo",               // 30
	
	// OCTOBER *******************************************************************************
	"Santa Teresa",               // 1
	"Santi Angeli custodi",       // 2
	"Sant' Edmondo",              // 3
	"S.Francesco d'Assisi",       // 4
	"San Placido,Faustina",       // 5
	"San Bruno",                  // 6
	"B.V.M. del Rosario",         // 7
	"Santa Pelagia",              // 8
	"San Dionigi",                // 9
	"San Daniele",                // 10
	"San Firmino",                // 11
	"San Serafino",               // 12
	"San Teofilo, Edoardo",       // 13
	"San Callisto",               // 14
	"Santa Teresa d'Avila",       // 15
	"Santa Margherita",           // 16
	"Sant' Ignazio",              // 17
	"San Luca evangelista",       // 18
	"Santa Laura",                // 19
	"Santa Irene, Aurora",        // 20
	"Sant' Orsola",               // 21
	"San Donato",                 // 22
	"S. Giovanni da C.",          // 23
	"S. Antonio Maria",           // 24
	"San Crispino",               // 25
	"San Evaristo",               // 26
	"San Fiorenzo",               // 27
	"S. Simone, Giuda",           // 28
	"Sant' Ermelinda",            // 29
	"San Marcello,Germano",       // 30
	"Santa Lucilla",              // 31
	
	// NOVEMBER ******************************************************************************
	"Ognissanti",                 // 1
	"Commem. defunti",            // 2
	"Santa Silvia",               // 3
	"San Carlo Borromeo",         // 4 - In Italy it's "Festa delle forze armate"
	"San Zaccaria",               // 5
	"San Leonardo",               // 6
	"Sant' Ernesto",              // 7
	"San Goffredo",               // 8
	"Sant' Oreste",               // 9
	"San Leone Magno",            // 10
	"S. Martino di Tours",        // 11
	"San Ranato, Giosafat",       // 12
	"Sant' Antioco, Diego",       // 13
	"San Giocondo",               // 14
	"Sant' Arturo",               // 15
	"Santa Margherita",           // 16
	"Santa Elisabetta",           // 17
	"Sant' Oddone",               // 18
	"Santa Matilde,Fausto",       // 19
	"San Ottavio",                // 20
	"Presentazione B.V.M.",       // 21
	"Santa Cecilia",              // 22
	"S. Clemente,Lucrezia",       // 23
	"Cristo Re",                  // 24
	"Santa Caterina",             // 25
	"San Corrado",                // 26
	"San Virgilio,Massimo",       // 27
	"San Giacomo",                // 28
	"San Saturnino",              // 29
	"S. Andrea apostolo",         // 30
	
	// DECEMBER ******************************************************************************
	"Sant'Eligio",                // 1
	"Santa Bibiana",              // 2
	"S. Francesco Saverio",       // 3
	"Santa Barbara",              // 4
	"San Giulio",                 // 5
	"San Nicola vescovo",         // 6
	"Sant'Ambrogio",              // 7
	"Immacolata Concezio.",       // 8
	"San Siro",                   // 9
	"B.V.M. di Loreto",           // 10
	"San Damaso I",               // 11
	"San Giovanna",               // 12
	"Santa Lucia",                // 13
	"S. Giovanni d. cr.",         // 14
	"San Valeriano",              // 15
	"Santa Albina",               // 16
	"San Lazzaro",                // 17
	"San Malachia",               // 18
	"San Dario di Nicea",         // 19
	"San Liberato",               // 20
	"San Pietro Canisio",         // 21
	"San Demetrio",               // 22
	"Santa Vittoria",             // 23
	"Santa Adele",                // 24
	"Natale del Signore",         // 25
	"Santo Stefano",              // 26
	"Santa Fabiola",              // 27
	"Santi Innocenti",            // 28
	"San Tommaso Becket",         // 29
	"Sant' Eugenio",              // 30
	"San Silvestro",              // 31
	};	
