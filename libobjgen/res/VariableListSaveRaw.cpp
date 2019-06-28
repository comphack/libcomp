([&]() -> bool
{
    @LENGTH_TYPE@ elementCount = static_cast<@LENGTH_TYPE@>(@VAR_NAME@.size());

    @STREAM@.write(reinterpret_cast<char*>(&elementCount),
        sizeof(elementCount));

    if(!@STREAM@.good())
    {
        return false;
    }

    for(auto& element : @VAR_NAME@)
    {
        if(!(@VAR_SAVE_CODE@))
        {
            return false;
        }
    }

    return @STREAM@.good();
})()
