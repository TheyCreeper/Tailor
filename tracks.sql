BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "tracks" (
	"id"	INTEGER NOT NULL,
	"filePath"	TEXT NOT NULL,
	"title"	TEXT NOT NULL,
	"albumId"	INTEGER,
	"trackNumber"	INTEGER,
	"durationSeconds"	INTEGER,
	"release"	INTEGER,
	"sampleRate"	INTEGER,
	"bitrate"	TEXT,
	"codec"	INTEGER,
	"format"	INTEGER,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("albumId") REFERENCES "album"("id")
);
COMMIT;
