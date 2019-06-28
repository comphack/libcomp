([&]() -> bool
{
    @LENGTH_TYPE@ elementCount = 0;

    @STREAM@.read(reinterpret_cast<char*>(&elementCount),
        sizeof(elementCount));

    if(!@STREAM@.good())
    {
        return false;
    }

    @PERSIST_COPY@
    @VAR_NAME@.clear();
    for(@LENGTH_TYPE@ i = 0; i < elementCount; ++i)
    {
        @VAR_TYPE@ element;

        if(!(@VAR_LOAD_CODE@))
        {
            return false;
        }

        @VAR_NAME@.push_back(element);
    }

    return @STREAM@.good();
})()
