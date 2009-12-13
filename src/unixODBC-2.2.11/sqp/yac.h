#define NAME 257
#define STRING 258
#define INTNUM 259
#define APPROXNUM 260
#define SELECT 261
#define FROM 262
#define WHERE 263
#define DELETE 264
#define INSERT 265
#define INTO 266
#define VALUES 267
#define UPDATE 268
#define SET 269
#define CURRENT 270
#define OF 271
#define CREATE 272
#define DROP 273
#define TABLE 274
#define ORDER 275
#define BY 276
#define ASC 277
#define DESC 278
#define CHAR 279
#define VARCHAR 280
#define LONG 281
#define NUMERIC 282
#define DECIMAL 283
#define SMALLINT 284
#define INTEGER 285
#define REAL 286
#define FLOAT 287
#define DOUBLE 288
#define PRECISION 289
#define BIT 290
#define TINYINT 291
#define BIGINT 292
#define BINARY 293
#define VARBINARY 294
#define DATE 295
#define TIME 296
#define TIMESTAMP 297
#define NULLX 298
#define IS 299
#define LIKE 300
#define ESCAPE 301
#define COMPARISON 302
#define OR 303
#define AND 304
#define NOT 305
typedef union {
	int intval;
	double floatval;
	char *strval;
	int subtok;
	HSQPCOMPARISON hComp;
	HSQPCOND hCond;
} YYSTYPE;
extern YYSTYPE yylval;
