* C++ ������ ��Ʈ��ũ �ھ�.

 - NetworkCore : ������(IOCP), ������(Epoll), �׽�Ʈ(Select) ���� �̿��� ��Ʈ��ũ ���̽� �ھ�.
 - NetworkAsio : Boost�� Asio�� �̿��� ��Ʈ��ũ ���̽� �ھ�. NetworkCore�� �������̽��� ����.
 - DatabaseConnector :  ODBC�� �̿��� ������ ���̽� ���� ������Ʈ. Mysql�� �̿��ؼ� �׽�Ʈ.
		        Redis ������ ���� CppRedis (https://github.com/Cylix/cpp_redis) Ŭ���̾�Ʈ�� ����. ��Ʈ��ũ ����� tacopie�� �����ϰ� Boost Asio�� �̿�.
			Json���� ���� ����� �����ϱ� ����JsonCpp ����.

 - TestServer : ��� �׽�Ʈ�� ���� ���� 
		�������� (0) : ���� �׽�Ʈ
		�������� (1) : Mysql ���� �׽�Ʈ

 - TestClient : ��� �׽�Ʈ�� ���� Ŭ���̾�Ʈ