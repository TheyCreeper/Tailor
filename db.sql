BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "album" (
	"id"	INTEGER NOT NULL UNIQUE,
	"name"	TEXT,
	"coverPath"	TEXT,
	"artistId"	INTEGER,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "albumTag" (
	"albumId"	INTEGER,
	"tagId"	INTEGER,
	FOREIGN KEY("albumId") REFERENCES "album"("id"),
	FOREIGN KEY("tagId") REFERENCES "tags"("id")
);
CREATE TABLE IF NOT EXISTS "artist" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT NOT NULL,
	"artPath"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "artistTag" (
	"artistId"	INTEGER,
	"tagId"	INTEGER,
	FOREIGN KEY("artistId") REFERENCES "artist"("id"),
	FOREIGN KEY("tagId") REFERENCES "tags"("id")
);
CREATE TABLE IF NOT EXISTS "genre" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "mood" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT,
	"color"	INTEGER,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "moodTag" (
	"moodId"	INTEGER,
	"tagId"	INTEGER UNIQUE,
	FOREIGN KEY("moodId") REFERENCES "mood"("id"),
	FOREIGN KEY("tagId") REFERENCES ""
);
CREATE TABLE IF NOT EXISTS "playlist" (
	"id"	INTEGER NOT NULL,
	"name"	TEXT,
	"favorite"	INTEGER DEFAULT 0,
	"coverPath"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "songTags" (
	"trackId"	INTEGER,
	"tagId"	INTEGER,
	FOREIGN KEY("tagId") REFERENCES "tags"("id"),
	FOREIGN KEY("trackId") REFERENCES "tracks"("id")
);
CREATE TABLE IF NOT EXISTS "tagPlaylist" (
	"tagId"	INTEGER,
	"playlistId"	INTEGER,
	FOREIGN KEY("playlistId") REFERENCES "playlist"("id"),
	FOREIGN KEY("tagId") REFERENCES "tags"("id")
);
CREATE TABLE IF NOT EXISTS "tags" (
	"id"	INTEGER NOT NULL,
	"text"	TEXT UNIQUE,
	"color"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
CREATE TABLE IF NOT EXISTS "trackGenre" (
	"trackId"	INTEGER,
	"genreId"	INTEGER,
	FOREIGN KEY("genreId") REFERENCES "genre"("id"),
	FOREIGN KEY("trackId") REFERENCES "tracks"("id")
);
CREATE TABLE IF NOT EXISTS "trackPlaylist" (
	"playlistId"	INTEGER,
	"trackId"	INTEGER,
	FOREIGN KEY("playlistId") REFERENCES "playlist"("id"),
	FOREIGN KEY("trackId") REFERENCES "tracks"("id")
);
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
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("albumId") REFERENCES "album"("id")
);
INSERT INTO "album" VALUES (1,NULL,NULL,NULL);
INSERT INTO "album" VALUES (2,NULL,NULL,NULL);
INSERT INTO "albumTag" VALUES (NULL,NULL);
INSERT INTO "artist" VALUES (1,'',NULL);
INSERT INTO "artistTag" VALUES (NULL,NULL);
INSERT INTO "mood" VALUES (1,NULL,NULL);
INSERT INTO "playlist" VALUES (1,NULL,0,NULL);
INSERT INTO "songTags" VALUES (NULL,NULL);
INSERT INTO "tagPlaylist" VALUES (NULL,NULL);
INSERT INTO "tags" VALUES (1,NULL,NULL);
INSERT INTO "trackPlaylist" VALUES (NULL,NULL);
INSERT INTO "tracks" VALUES (1,'','',NULL,NULL,NULL,NULL,NULL,NULL);
COMMIT;
