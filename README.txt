* ���� ȯ��
 - Visual studio 2017 ���� 15.4.3
 - Windows SDK ���� : 10.0.16299.0 ( 8.1 ���������� ���尡 ���� ���� ���� �ֽ��ϴ�. )
 - Boost 1.64 
	: ������Ʈ�� User Macro�� $(BoostInclude) �� Boost ��θ�, $(BoostLib) �� lib ��θ� �����ؾ� �մϴ�.

* PacketGen
 - Python�� �̿��� ��Ŷ ���ʷ�����
 - ���ǵ� Json ������ �о Protocol �� Encode, Decode�� ����� ���ϴ� ��ο� �����մϴ�.
 - �׽�Ʈ�� ���������� ���ǵǾ� �ֽ��ϴ�. ���� �ڷ����� Echo �� �� �ִ� �������ݰ� DB ������ �׽�Ʈ �� �� �ִ� ���������� ���ǵǾ� �ֽ��ϴ�.

* C++ ������ ��Ʈ��ũ �ھ�.

 - NetworkCore : ������(IOCP), ������(Epoll), �׽�Ʈ(Select) ���� �̿��� ��Ʈ��ũ ���̽� �ھ�.
 - NetworkAsio : Boost�� Asio�� �̿��� ��Ʈ��ũ ���̽� �ھ�. NetworkCore�� �������̽��� ����.
 - DatabaseConnector :  ODBC�� �̿��� ������ ���̽� ���� ������Ʈ. Mysql�� �̿��ؼ� �׽�Ʈ.
				Redis ������ ���� CppRedis (https://github.com/Cylix/cpp_redis) Ŭ���̾�Ʈ�� ����. ��Ʈ��ũ ����� tacopie�� �����ϰ� Boost Asio�� �̿�.
				Json���� ���� ����� �����ϱ� ����JsonCpp ����.

 - TestServer : ��� �׽�Ʈ�� ���� ���� 
 - TestClient : ��� �׽�Ʈ�� ���� Ŭ���̾�Ʈ

