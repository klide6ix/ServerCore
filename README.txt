* ���� ȯ��
 - Visual studio 2019 ���� 16.8.3
 - Windows SDK ���� : 10.0.18362.0
 - Boost 1.74 
	: set_boost_path.bat �� �̿��� �ڽ��� boost ��θ� ȯ�溯���� �����ؾ� �Ѵ�.

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

