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
	"codec"	INTEGER,
	"format"	INTEGER,
	PRIMARY KEY("id" AUTOINCREMENT),
	FOREIGN KEY("albumId") REFERENCES "album"("id")
);
COMMIT;
