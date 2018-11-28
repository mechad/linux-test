namespace cpp Chad

enum Rtype {
	BOOL = 0,
	INTEGER = 1,
	STRING = 2;
	SET = 3,
	MAP = 4,
	LIST = 5,
	BIN = 6,
}
struct IOArgs {
	1: Rtype type,
	2: bool status,
	3: i32 i,
	4: set<string> s,
	5: map<string,string> m,
	6: list<string> l,
	7: binary b,
}

exception InvalidOperation {
  1: i32 whatOp,
  2: string why
}

service OperationService {
	bool Ping(),
	string Set(map<string,string> m) throws (1:InvalidOperation ouch),
	map<string,string> Get(list<string> l) throws (1:InvalidOperation ouch),
    IOArgs Op(1: IOArgs io_args) throws (1:InvalidOperation ouch)
}
