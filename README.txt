* C++ 서버용 네트워크 코어.

 - NetworkCore : 윈도우(IOCP), 리눅스(Epoll), 테스트(Select) 모델을 이용한 네트워크 베이스 코어.
 - NetworkAsio : Boost의 Asio를 이용한 네트워크 베이스 코어. NetworkCore와 인터페이스를 맞춤.
 - DatabaseConnector :  ODBC를 이용한 데이터 베이스 연결 프로젝트. Mysql을 이용해서 테스트.
		        Redis 접속을 위한 CppRedis (https://github.com/Cylix/cpp_redis) 클라이언트를 적용. 네트워크 통신은 tacopie를 제거하고 Boost Asio를 이용.
			Json으로 쿼리 결과를 리턴하기 위해JsonCpp 적용.

 - TestServer : 모듈 테스트를 위한 서버 
		프로토콜 (0) : 에코 테스트
		프로토콜 (1) : Mysql 쿼리 테스트

 - TestClient : 모듈 테스트를 위한 클라이언트