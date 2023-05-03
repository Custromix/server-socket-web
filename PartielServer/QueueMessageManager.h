#pragma once
class QueueMessageManager
{
public:

    QueueMessageManager();
    ~QueueMessageManager();

    void EnqueueMessage(const string& message, SClient* client);

    bool IsEmpty() const;

    SMessage DequeueMessage();


private:
    queue<SMessage> messages_;
    mutable mutex mutex_;
};

