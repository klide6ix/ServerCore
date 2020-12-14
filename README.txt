* 빌드 환경
 - Visual studio 2019 버전 16.8.3
 - Windows SDK 버전 : 10.0.18362.0
 - Boost 1.74 
	: set_boost_path.bat 를 이용해 자신의 boost 경로를 환경변수로 지정해야 한다.

* PacketGen
 - Python을 이용한 패킷 제너레이터
 - 정의된 Json 형식을 읽어서 Protocol 과 Encode, Decode를 만들어 원하는 경로에 복사합니다.
 - 테스트용 프로토콜이 정의되어 있습니다. 각종 자료형을 Echo 할 수 있는 프로토콜과 DB 연결을 테스트 할 수 있는 프로토콜이 정의되어 있습니다.

* C++ 서버용 네트워크 코어.

 - NetworkCore : 윈도우(IOCP), 리눅스(Epoll), 테스트(Select) 모델을 이용한 네트워크 베이스 코어.
 - NetworkAsio : Boost의 Asio를 이용한 네트워크 베이스 코어. NetworkCore와 인터페이스를 맞춤.
 - DatabaseConnector :  ODBC를 이용한 데이터 베이스 연결 프로젝트. Mysql을 이용해서 테스트.
				Redis 접속을 위한 CppRedis (https://github.com/Cylix/cpp_redis) 클라이언트를 적용. 네트워크 통신은 tacopie를 제거하고 Boost Asio를 이용.
				Json으로 쿼리 결과를 리턴하기 위해JsonCpp 적용.

 - TestServer : 모듈 테스트를 위한 서버 
 - TestClient : 모듈 테스트를 위한 클라이언트

